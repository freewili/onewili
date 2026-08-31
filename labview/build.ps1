<#
.SYNOPSIS
    Build onewili_lv.dll for 64-bit and 32-bit LabVIEW.

.DESCRIPTION
    Produces bin\win64\onewili_lv.dll and bin\win32\onewili_lv.dll. Both have
    the same file name on purpose: a Call Library Function Node that refers to
    "onewili_lv.dll" by name alone picks up whichever copy sits next to the
    VIs, so one VI library works in either LabVIEW bitness once you drop in
    the matching DLL.

    Needs CMake and Visual Studio with the C++ workload (Build Tools is
    enough). The MSVC environment is set up through vcvarsall.bat and the
    build runs with "NMake Makefiles", so this does not care whether your
    CMake is new enough to know your Visual Studio version.

.EXAMPLE
    .\build.ps1
    .\build.ps1 -Arch x64 -Config Debug
    .\build.ps1 -Regenerate          # re-run the API generator first
    .\build.ps1 -SelfTestPort COM7   # also run the device half of the self-test
    .\build.ps1 -Tests               # also build bin\test and run pytest
    .\build.ps1 -Tests -SelfTestPort auto   # ...including the hardware tests
#>
[CmdletBinding()]
param(
    [ValidateSet('both', 'x64', 'x86')] [string] $Arch = 'both',
    [ValidateSet('Release', 'Debug')]   [string] $Config = 'Release',
    [string] $SelfTestPort,
    [switch] $Regenerate,
    [switch] $Tests
)

$ErrorActionPreference = 'Stop'
$root = $PSScriptRoot

function Find-VcVarsAll {
    $vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
    if (-not (Test-Path $vswhere)) { throw "vswhere.exe not found; is Visual Studio installed?" }
    $install = & $vswhere -latest -products * `
        -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
        -property installationPath
    if (-not $install) { throw "no Visual Studio with the C++ tools found" }
    $bat = Join-Path $install 'VC\Auxiliary\Build\vcvarsall.bat'
    if (-not (Test-Path $bat)) { throw "vcvarsall.bat not found under $install" }
    return $bat
}

# Run vcvarsall for one architecture and pull its environment back into this
# process. Everything after this call compiles for that target.
function Enter-MsvcEnv([string] $vcvarsall, [string] $target) {
    $out = cmd /c "`"$vcvarsall`" $target >nul 2>&1 && set"
    if ($LASTEXITCODE -ne 0) { throw "vcvarsall.bat $target failed" }
    foreach ($line in $out) {
        if ($line -match '^([^=]+)=(.*)$') {
            Set-Item -Path ("env:" + $matches[1]) -Value $matches[2] -ErrorAction SilentlyContinue
        }
    }
}

if ($Regenerate) {
    Write-Host "regenerating the API..." -ForegroundColor Cyan
    python (Join-Path $root 'tools\gen_lv_api.py')
    if ($LASTEXITCODE -ne 0) { throw "generator failed" }
}

$vcvarsall = Find-VcVarsAll
$targets = if ($Arch -eq 'both') { @('x64', 'x86') } else { @($Arch) }
$outer = [System.Collections.Generic.Dictionary[string, string]]::new()
foreach ($e in [System.Environment]::GetEnvironmentVariables().GetEnumerator()) {
    $outer[$e.Key] = [string]$e.Value
}

foreach ($a in $targets) {
    $build  = Join-Path $root "build\$a"
    $outDir = Join-Path $root ("bin\" + $(if ($a -eq 'x64') { 'win64' } else { 'win32' }))

    Write-Host "`n=== $a / $Config ===" -ForegroundColor Cyan

    # Each pass starts from the environment this script was launched with, so
    # the x86 pass does not inherit the x64 pass's PATH/LIB/INCLUDE.
    Get-ChildItem env: | ForEach-Object {
        if (-not $outer.ContainsKey($_.Name)) { Remove-Item ("env:" + $_.Name) -ErrorAction SilentlyContinue }
    }
    foreach ($kv in $outer.GetEnumerator()) { Set-Item ("env:" + $kv.Key) $kv.Value -ErrorAction SilentlyContinue }
    Enter-MsvcEnv $vcvarsall $a

    Remove-Item -Recurse -Force $build -ErrorAction SilentlyContinue
    cmake -S $root -B $build -G "NMake Makefiles" `
          -DCMAKE_BUILD_TYPE=$Config -DONEWILI_LV_SELFTEST=ON
    if ($LASTEXITCODE -ne 0) { throw "cmake configure failed for $a" }

    cmake --build $build
    if ($LASTEXITCODE -ne 0) { throw "build failed for $a" }

    New-Item -ItemType Directory -Force -Path $outDir | Out-Null
    foreach ($f in 'onewili_lv.dll', 'onewili_lv.lib', 'owlv_selftest.exe') {
        Copy-Item (Join-Path $build $f) $outDir -Force
    }
    Write-Host "  -> $outDir" -ForegroundColor Green

    $exe = Join-Path $outDir 'owlv_selftest.exe'
    if ($SelfTestPort) { & $exe $SelfTestPort } else { & $exe }
    if ($LASTEXITCODE -ne 0) { throw "self-test failed for $a" }
}

if ($Tests) {
    # A second x64 build with the scripted loopback transport compiled in.
    # The 540 command forwarders are identical to the shipped build; only the
    # transport differs, so the round-trip tests cover code that ships.
    Write-Host "`n=== test build (x64, loopback) ===" -ForegroundColor Cyan
    Get-ChildItem env: | ForEach-Object {
        if (-not $outer.ContainsKey($_.Name)) { Remove-Item ("env:" + $_.Name) -ErrorAction SilentlyContinue }
    }
    foreach ($kv in $outer.GetEnumerator()) { Set-Item ("env:" + $kv.Key) $kv.Value -ErrorAction SilentlyContinue }
    Enter-MsvcEnv $vcvarsall 'x64'

    $tb = Join-Path $root 'build\test'
    $td = Join-Path $root 'bin\test'
    Remove-Item -Recurse -Force $tb -ErrorAction SilentlyContinue
    cmake -S $root -B $tb -G "NMake Makefiles" `
          -DCMAKE_BUILD_TYPE=$Config -DONEWILI_LV_LOOPBACK=ON
    if ($LASTEXITCODE -ne 0) { throw "cmake configure failed for the test build" }
    cmake --build $tb
    if ($LASTEXITCODE -ne 0) { throw "test build failed" }
    New-Item -ItemType Directory -Force -Path $td | Out-Null
    Copy-Item (Join-Path $tb 'onewili_lv.dll') $td -Force

    # conftest.py prefers bin\test, so the loopback suite runs by default.
    $pytestArgs = @('-m', 'pytest', (Join-Path $root 'tests'), '-q')
    if ($SelfTestPort) { $pytestArgs += @('--port', $SelfTestPort) }
    Write-Host "`n=== pytest ===" -ForegroundColor Cyan
    python @pytestArgs
    if ($LASTEXITCODE -ne 0) { throw "tests failed" }
}

Write-Host "`nDone. Copy the DLL matching your LabVIEW bitness next to your VIs." -ForegroundColor Green

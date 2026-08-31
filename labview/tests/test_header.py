"""The header is the contract with LabVIEW's Import Shared Library Wizard.

These tests need no DLL and no hardware. They check the property the whole
design rests on: every declaration the wizard will read is expressible in a
Call Library Function Node, and the export table matches it exactly.
"""

from __future__ import annotations

import re
import subprocess
import sys
from pathlib import Path

import pytest

from conftest import API_HEADER, CORE_HEADER, DEF_FILE, LV, declarations

# The complete set of parameter spellings a Call Library Function Node can
# express without ambiguity. Anything else -- stdint types, size_t, bool,
# enums, structs, function pointers -- is a wizard failure or, worse, a
# silently wrong marshalling.
ALLOWED = {
    "int", "unsigned int", "unsigned char", "double",
    "int*", "unsigned int*", "unsigned char*", "double*",
    "char*", "const char*", "const unsigned char*",
}


def test_headers_declare_only_labview_safe_types():
    offenders = []
    for name, params in declarations().items():
        for p in params:
            if p not in ALLOWED:
                offenders.append(f"{name}: {p!r}")
    assert not offenders, (
        "these parameters cannot be expressed in a Call Library Function "
        "Node:\n  " + "\n  ".join(offenders))


def test_headers_pull_in_no_system_includes():
    """The wizard parses the header on its own; a <stdint.h> it cannot find
    makes it drop every function that mentions one of those types."""
    for header in (CORE_HEADER, API_HEADER):
        text = header.read_text(encoding="utf-8")
        assert "#include <" not in text, f"{header.name} includes a system header"
    core = CORE_HEADER.read_text(encoding="utf-8")
    assert '#include "onewili_lv_api.h"' in core


def test_no_macros_inside_declarations():
    """OWLV_API-style export macros are the usual reason the wizard skips a
    function. There are none here; this keeps it that way."""
    text = re.sub(r"/\*.*?\*/", " ", API_HEADER.read_text(encoding="utf-8"), flags=re.S)
    for line in text.splitlines():
        line = line.strip()
        if line.startswith("int owlv_"):
            assert not re.search(r"\b[A-Z][A-Z0-9_]{2,}\b", line), \
                f"macro in a declaration: {line}"


def test_export_list_matches_the_headers():
    exports = [l.strip() for l in DEF_FILE.read_text(encoding="utf-8").splitlines()
               if l.startswith("    ")]
    declared = set(declarations())
    # owlv_pointer_size returns a size, not a status, so it is not in decls()
    declared.add("owlv_pointer_size")
    assert len(exports) == len(set(exports)), "duplicate names in onewili_lv.def"
    assert set(exports) == declared, (
        f"only in .def: {sorted(set(exports) - declared)}\n"
        f"only in headers: {sorted(declared - set(exports))}")


def test_every_command_is_documented():
    api = [m.group(1) for m in
           re.finditer(r"^int (owlv_\w+)\(", API_HEADER.read_text(encoding="utf-8"), re.M)]
    docs = (LV / "docs" / "commands.md").read_text(encoding="utf-8")
    documented = set(re.findall(r"\| `(owlv_\w+)` \|", docs))
    assert set(api) == documented
    assert len(api) == len(set(api)), "a command name is declared twice"


def test_generator_is_deterministic(tmp_path):
    """Re-running the generator must not change a byte. If it does, a review
    cannot tell a real API change from generator churn."""
    before = {p: p.read_bytes() for p in [
        API_HEADER,
        LV / "src" / "onewili_lv_api.c",
        DEF_FILE,
        LV / "docs" / "commands.md",
    ]}
    r = subprocess.run([sys.executable, str(LV / "tools" / "gen_lv_api.py")],
                       capture_output=True, text=True)
    assert r.returncode == 0, r.stderr
    changed = [p.name for p, b in before.items() if p.read_bytes() != b]
    assert not changed, f"regenerating changed: {changed}"


def test_generator_rejects_an_unknown_parameter_shape(tmp_path):
    """The generator must fail loudly on a type it does not understand rather
    than emit a forwarder that marshals it wrong."""
    sys.path.insert(0, str(LV / "tools"))
    try:
        import gen_lv_api
    finally:
        sys.path.pop(0)
    with pytest.raises(SystemExit) as e:
        gen_lv_api.classify([("float*", "gain")], set(), "ow_made_up")
    assert "gain" in str(e.value) and "float*" in str(e.value)

"""File System menu - generated from fwMenuFileSystem. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class FileSystem(MenuBase):
    r"""File System (``h\x``)."""

    #: Spontaneous event frames this menu emits (match Transport.events
    #: frames by id). payload lists (name, wire_type) pairs.
    EVENTS = {
        "fdir": {"binary": False, "payload": [("kind", "string"), ("name", "string"), ("size", "decU32")], "description": "directory listing entry; kind is dir/fil, or end with size as the entry count"},
        "filedl": {"binary": False, "payload": [("data", "string")], "description": "File download progress ('complete N bytes')"},
        "fpgadl": {"binary": False, "payload": [("data", "string")], "description": "FPGA bitstream download progress ('complete N bytes')"},
    }

    def change_directory(self, path: str) -> Result:
        r"""Change Directory.

        Wire: ``h\x\a``

        Changes current directory

        Directory Name

        Args:
            path: path (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [encoding.enc_str(path)], [])

    def create_directory(self, path: str) -> Result:
        r"""Create Directory.

        Wire: ``h\x\c``

        Creates a new directory

        Directory Name

        Args:
            path: path (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [encoding.enc_str(path)], [])

    def remove_file_or_directory(self, path: str) -> Result:
        r"""Remove File or Directory.

        Wire: ``h\x\r``

        Removes a file or directory

        File or Directory Name

        Args:
            path: path (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("r", [encoding.enc_str(path)], [])

    def get_file_from_pc(self, path: str, size: int, crc32: int) -> Result:
        r"""Get File From PC.

        Wire: ``h\x\f``

        Downloads file to Free Wili

        Enter File Path, File Size, and crc32 checksum separated by a space

        Args:
            path: path (string).
            size: size (decS32).
            crc32: crc32 (decU32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("f", [encoding.enc_str(path), encoding.enc_int(size), encoding.enc_int(crc32)], [])

    def send_file_to_pc(self, path: str) -> Result:
        r"""Send File To PC.

        Wire: ``h\x\u``

        Sends file to PC

        Enter FilePath

        Args:
            path: path (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("u", [encoding.enc_str(path)], [])

    def print_file(self, path: str) -> Result:
        r"""Print File.

        Wire: ``h\x\p``

        Prints the File Content

        Enter FilePath

        Args:
            path: path (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("p", [encoding.enc_str(path)], [])

    def create_blank_file(self, path: str) -> Result:
        r"""Create Blank File.

        Wire: ``h\x\b``

        Creates a blank file

        Enter FileName

        Args:
            path: path (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("b", [encoding.enc_str(path)], [])

    def edit_file(self, path: str) -> Result:
        r"""Edit File.

        Wire: ``h\x\e``

        Edits a text file

        Enter FileName

        Args:
            path: path (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("e", [encoding.enc_str(path)], [])

    def rename_or_move_file_directory(self, path: str, new_path: str) -> Result:
        r"""Rename or Move File Or Directory.

        Wire: ``h\x\n``

        Renames or Moves a File or Directory

        File or Directory Names Separated by Spaces

        Args:
            path: path (string).
            new_path: new_path (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("n", [encoding.enc_str(path), encoding.enc_str(new_path)], [])

    def list_directory(self, path: str) -> Result:
        r"""List Directory.

        Wire: ``h\x\l``

        lists the contents of a directory. Blank for current directory.

        Directory path or blank for current directory

        Args:
            path: path (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("l", [encoding.enc_str(path)], [])

    def format_file_system(self, confirm: str) -> Result:
        r"""Format File System.

        Wire: ``h\x\t``

        reformats the internal flash

        DANGER. Erases Everything. Type destroyfiles to continue. This will take time.

        Args:
            confirm: confirm (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("t", [encoding.enc_str(confirm)], [])

    def toggle_sd_card_host_select(self) -> Result:
        r"""Toggle SDCard Host.

        Wire: ``h\x\s``

        Toggles which host controls the SD card.

        Toggles which host controls the SD card.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [], [])

    def load_wili_project(self, path: str) -> Result:
        r"""Load Wili Project.

        Wire: ``h\x\w``

        Loads a fwcom .wili project (panels, blocks, app signals) and shows the Panels app.

        Enter .wili Project FilePath

        Args:
            path: path (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("w", [encoding.enc_str(path)], [])

    def set_sd_card_host(self, host: int) -> Result:
        r"""SDCard Host Select.

        Wire: ``h\x\k``

        Connects the SD card to the main CPU (0) or the USB reader / PC (1).

        0=main cpu, 1=usb reader

        Args:
            host: host (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("k", [encoding.enc_int(host)], [])

    def begin_file_read(self, session: int, path_hex: str) -> Result:
        r"""Begin File Read.

        Wire: ``h\x\0``

        Open an SD file for bounded framed reads. Paths are UTF-8 encoded as compact hex and must be absolute; the session expires after 30 seconds of inactivity.

        session=hex32,path_hex=string

        Args:
            session: session (hex32).
            path_hex: path_hex (string).

        Returns:
            Result: Ok(size: int) or Err(message).
        """
        return self._call("0", [encoding.enc_hex(session, 8), encoding.enc_str(path_hex)], ["int"])

    def begin_file_write(self, session: int, path_hex: str, size: int, crc32: int, overwrite: bool) -> Result:
        r"""Begin File Write.

        Wire: ``h\x\1``

        Stage an upload beside its destination. Existing files are preserved until size and CRC32 validation succeeds. No raw USB mode is entered.

        session=hex32,path_hex=string,size=decU32,crc32=hex32,overwrite=bool

        Args:
            session: session (hex32).
            path_hex: path_hex (string).
            size: size (decU32).
            crc32: crc32 (hex32).
            overwrite: overwrite (bool).

        Returns:
            Result: Ok(size: int) or Err(message).
        """
        return self._call("1", [encoding.enc_hex(session, 8), encoding.enc_str(path_hex), encoding.enc_int(size), encoding.enc_hex(crc32, 8), encoding.enc_bool(overwrite)], ["int"])

    def read_file_chunk(self, session: int, offset: int, maximum: int) -> Result:
        r"""Read File Chunk.

        Wire: ``h\x\2``

        Read the next 1 to 192 bytes as compact hex. Use the exact sequential offset; a dash means an empty file or EOF.

        session=hex32,offset=decU32,maximum=decU32

        Args:
            session: session (hex32).
            offset: offset (decU32).
            maximum: maximum (decU32).

        Returns:
            Result: Ok(count: int, data: str) or Err(message).
        """
        return self._call("2", [encoding.enc_hex(session, 8), encoding.enc_int(offset), encoding.enc_int(maximum)], ["int", "str"])

    def write_file_chunk(self, session: int, offset: int, data: str) -> Result:
        r"""Write File Chunk.

        Wire: ``h\x\3``

        Write the next 1 to 192 hex-encoded bytes. Duplicate or out-of-order chunks are rejected; never replay an ambiguous timeout.

        session=hex32,offset=decU32,data=string

        Args:
            session: session (hex32).
            offset: offset (decU32).
            data: data (string).

        Returns:
            Result: Ok(position: int) or Err(message).
        """
        return self._call("3", [encoding.enc_hex(session, 8), encoding.enc_int(offset), encoding.enc_str(data)], ["int"])

    def finish_file_transfer(self, session: int) -> Result:
        r"""Finish File Transfer.

        Wire: ``h\x\4``

        Verify the byte count, close the file and return CRC32. A complete verified upload is published; an incomplete or corrupt upload never replaces the destination.

        session=hex32

        Args:
            session: session (hex32).

        Returns:
            Result: Ok(size: int, crc32: int) or Err(message).
        """
        return self._call("4", [encoding.enc_hex(session, 8)], ["int", "hex"])

    def cancel_file_transfer(self, session: int) -> Result:
        r"""Cancel File Transfer.

        Wire: ``h\x\5``

        Close the matching transfer and remove its incomplete staging file. Other shell and menu sessions remain available.

        session=hex32

        Args:
            session: session (hex32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("5", [encoding.enc_hex(session, 8)], [])

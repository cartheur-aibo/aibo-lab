# ODA File Format

This is a cleaned-up Markdown edition of `ODA-FFORM-E.txt`.

## Purpose

The `ODA` format stores content archive information.

## Basic Properties

- binary format
- byte order: little endian
- extension: `.ODA`
- magic: `ODAR`

## Header

The header contains:

- 4-byte magic
- major version
- minor version
- number of files
- table element size
- reserved bytes
- data section

## Table Entries

Each file entry contains:

- type
- file name
- offset
- size
- reserved field

The format note says entry types may include values such as:

- `WAVE`
- `MIDI`
- `OMTN`

## Meaning

An `ODA` file behaves like a simple asset archive that can contain
multiple files used by the runtime.

## Use In This Repo

This matters because R-Code and performer assets include `.ODA` files
for motions, audio, and system data inside the preserved stick image.


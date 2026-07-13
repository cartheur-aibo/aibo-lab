# MTN File Format

This is a cleaned-up Markdown edition of `MTN-FFORM-E.txt`.

## Purpose

The `MTN` format stores motion data information for the robot.

## Basic Properties

- byte order: little endian
- magic: `OMTN`

## Layout Overview

The file begins with:

- 4-byte magic
- Section 0
- Section 1
- Section 2
- Section 3

## Section 0

Fixed-length header containing:

- section number
- section size
- number of sections
- major version
- minor version
- number of keyframes
- frame rate
- reserved field

## Section 1

Variable-length naming section containing:

- motion name
- creator
- design label
- optional padding

## Section 2

Variable-length joint list containing:

- number of joints
- a list of joint primitive locator names
- optional padding

## Section 3

The text indicates this section describes the type of motion data and is
variable-length.

## Use In This Repo

This format note helps explain motion assets referenced by R-Code and
`AIBO Performer` output.


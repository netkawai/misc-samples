In NAND, ECC use BCH
in 512 bytes block, requires bits are requied correction bit level (4,8,16bits) * 13bits

ECC bits requires for a 512 byte data block is (ECC_correction_level * 13 bits), a maximum ECC correction level = 16 can be chosen in theory.

224 = 8 * (26 + 2)
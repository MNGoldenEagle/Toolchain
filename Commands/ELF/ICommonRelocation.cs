using ELFSharp.ELF.Sections;

namespace ELF
{
    public interface ICommonRelocation
    {
        uint Offset { get; set; }

        SymbolEntry<uint> Symbol { get; set; }

        RelocationType Type { get; set; }
    }

    public enum RelocationType : ushort
    {
        MIPS_NONE = 0,
        MIPS_16 = 1,
        MIPS_32 = 2,
        MIPS_REL32 = 3,
        MIPS_26 = 4,
        MIPS_HI16 = 5,
        MIPS_LO16 = 6,
        MIPS_GPREL16 = 7,
        MIPS_LITERAL = 8,
        MIPS_GOT16 = 9,
        MIPS_PC16 = 10,
        MIPS_CALL16 = 11,
        MIPS_GPREL32 = 12
    }
}

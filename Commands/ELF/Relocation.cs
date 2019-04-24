using ELFSharp.ELF.Sections;
using MiscUtil.Conversion;
using System.Linq;

namespace ELF
{
    public class Relocation : ICommonRelocation
    {
        private static readonly BigEndianBitConverter CONVERTER = new BigEndianBitConverter();

        public const int SIZE = 8;

        public uint Offset { get; set; }

        public SymbolEntry<uint> Symbol { get; set; }

        public RelocationType Type { get; set; }

        public Relocation(byte[] data, int start, SymbolTable<uint> symbols)
        {
            Offset = CONVERTER.ToUInt32(data, start);

            var info = CONVERTER.ToUInt32(data, start + 4);

            var symbolTargetIndex = info >> 8;
            Symbol = symbols.Entries.ElementAt((int) symbolTargetIndex);
            Type = (RelocationType)(info & 0x000000FF);
        }
    }
}

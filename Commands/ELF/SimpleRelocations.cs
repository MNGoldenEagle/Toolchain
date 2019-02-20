using ELFSharp.ELF.Sections;

namespace ELF
{
    public class SimpleRelocationSection
    {
        private readonly ISection Section;

        public Relocation[] Relocations;

        public SimpleRelocationSection(ISection section, SymbolTable<uint> symbols)
        {
            Section = section;

            var count = section.GetContents().Length / Relocation.SIZE;

            Relocations = new Relocation[count];

            for (int i = 0, start = 0; i < count; i++, start += Relocation.SIZE)
            {
                Relocations[i] = new Relocation(section.GetContents(), start, symbols);
            }
        }
    }
}

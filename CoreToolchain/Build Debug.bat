@ECHO OFF
dotnet build -f net462 -c Debug -r win10-x64
dotnet build -f netcoreapp2.1 -c Debug -r osx-x64
dotnet build -f netcoreapp2.1 -c Debug -r linux-x64
dotnet publish -f net462 -c Debug -r win10-x64
dotnet publish -f netcoreapp2.1 -c Debug -r osx-x64
dotnet publish -f netcoreapp2.1 -c Debug -r linux-x64
robocopy ".\Binaries-Win10" "bin\Debug\net462\win10-x64\publish\Binaries" *.* /NP /NJH
robocopy ".\Binaries-OSX" "bin\Debug\netcoreapp2.1\osx-x64\publish\Binaries" *.* /NP /NJH
robocopy ".\Binaries-Linux" "bin\Debug\netcoreapp2.1\linux-x64\publish\Binaries" *.* /NP /NJH
robocopy ".\Templates" "bin\Debug\net462\win10-x64\publish\Templates" *.* /NP /NJH
robocopy ".\Templates" "bin\Debug\netcoreapp2.1\osx-x64\publish\Templates" *.* /NP /NJH
robocopy ".\Templates" "bin\Debug\netcoreapp2.1\linux-x64\publish\Templates" *.* /NP /NJH
robocopy "..\Z64 Headers" "bin\Debug\net462\win10-x64\publish\Headers" *.h /NP /NJH
robocopy "..\Z64 Headers" "bin\Debug\netcoreapp2.1\osx-x64\publish\Headers" *.h /NP /NJH
robocopy "..\Z64 Headers" "bin\Debug\netcoreapp2.1\linux-x64\publish\Headers" *.h /NP /NJH
robocopy "..\Z64 Headers\examples" "bin\Debug\net462\win10-x64\publish\Examples" *.* /E /NP /NJH
robocopy "..\Z64 Headers\examples" "bin\Debug\netcoreapp2.1\osx-x64\publish\Examples" *.* /E /NP /NJH
robocopy "..\Z64 Headers\examples" "bin\Debug\netcoreapp2.1\linux-x64\publish\Examples" *.* /E /NP /NJH

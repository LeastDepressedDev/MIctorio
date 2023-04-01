# MIctorio
`MIctorio` - Mod Interface for Factorio.
Programm developed by Sirtage to simplify factorio mod development proccess.

# Components
This mod interface contains:
- custom - Custom element without user interface
- wit - `Without initialised type` component
- virt - virtual type
- hpar - `Uninited type`'s class with parent
- c_item - item type
- c_recipe - recipe type
components.

# Compilator
Built-in compilator works stable on every release, that means you can easily compile your mod without getting troubles and blah-blah-blah

# ~~Gui~~
**Not comming soon**

# Project structure
If you need to remove component manualy, you should firstly delete it from project structure! Project structure stays in `index.prj` file inside your project's folder.
Lines which starts with `fstruct@**f:;**` is means, that line contains info about project's element. So if you want to delete this element from the project, just remove this line.
Line example: `f:TestRecipe@recipe:rcp.cmp;`
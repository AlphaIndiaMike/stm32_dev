# STM32_DEV

STM32 CMAKE starter project

# Project File Structure

The project’s folders are defined in depth according to the following hierarchy:

1. **Domain (Item)**
2. **Subsystem**
3. **Software Component (Module)**
4. **Software Units (Unit)**

Each level is prefixed with a numeric range or name, as indicated below.

---

## Example Folder Tree

```plaintext
.
├── 00_reserved/      <-- Reserved domain (00..09)
│
├── 10_startup/       <-- Start-up domain (10_fbl, 11_csm ..)
│   ├── 10_bsp/       <-- example: BSP
│   │   
│   ├── 11_os/        <-- example: os subsystem
│   │   
│   ├── 20_core_0
│   │   └── 00_main/  <-- Mandatory: entry module
│   │       └── init_unit/    <-- Example unit
│   │           ├── Src/      <-- Source (C/CPP)
│   │           ├── Inc/      <-- Headers (H/HPP)
│   │           └── Test/     <-- Test case source
│   │
│   ├── 21_core_1/    <-- same structure as core 0
│   │ 
│   ├── (other subsystems)
│   │ 
│   └── 90_common     <-- same structure as core 0
│
├── 40_application/   <-- Application software domain (40..80)
│   ├── .. same pattern as startup ..
│   │
│   ├── 20_core_0/    <-- core / host-specific subsystem (20..89)
│   │   ├── 00_main/    <-- Mandatory module
│   │   │   └── core_init/    <-- Example unit
│   │   │       ├── Src/
│   │   │       ├── Inc/
│   │   │       └── Test/
│   │   └── SWC_name/      <-- Component/Module
│   │       └── SWC_unit/     <-- Unit
│   │           ├── Src/
│   │           ├── Inc/
│   │           └── Test/
│   └── (other subsystems)/
│
├── 81_reserved/                     <-- Reserved domain (81..89)
│   └── (contents)/
│
├── 90_config/                       <-- Configuration domain
│   └── (contents)/
│
├── 91_HIL/                          <-- HIL test setup domain
│   └── (contents)/
│
├── 92_SiL/                          <-- Software in a Loop test setup domain
│   └── (contents)/
│
├── 99_build/                        <-- Build setup domain
│   └── (contents)/
│
└── build.sh    <-- Project build script
```

---

## Explanation

1. **Domain (Item)**  
   - Use numeric prefixes to indicate the type of domain (e.g., `00..09` for reserved, `10..39` for start-up, `40..80` for application software, etc.).  
   - Environment specific, e.g. `90_config`, `99_build`

2. **Subsystem**  
   - Within each domain, create subsystems also using numeric prefixes (e.g., `10_bsp`, `11_os`, `20_core`).  
   - These subsystem folders group related modules / components.

3. **Software Component (Module)**  
   - Each subsystem can have one or more modules. The mandatory module is named `00_main`, which contains initialization, “entry” code, or other foundational logic.  
   - Additional modules follow your project naming convention (e.g., `01_scheduler`, `01_abc_manager`).

4. **Software Units (Unit)**  
   - Each module is further divided into one or more units.  
   - A unit typically has subfolders for source code (`Src/`), headers (`Inc/`), and tests (`Test/`).


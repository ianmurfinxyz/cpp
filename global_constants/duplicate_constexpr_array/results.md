## Results of Using inline Variables

Replacing the line,

```c++
constexpr std::array<const char*, 500> g_strings {
  ...
```

with,

```c++
inline constexpr std::array<const char*, 500> g_strings {
  ...
```

resulted in a reduction of resultant ELF size from 66504 bytes (~65kib) (without inline 
variables) to 33680 bytes (~33kib). A 2:1 size difference! Clearly significant savings can be made 
if you require large compile time constant data sets. Pretty cool :smile:


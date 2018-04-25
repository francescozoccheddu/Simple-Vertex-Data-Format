# Simple Vertex Data Format
## Data format specification v0.1

### Comment
+ _`[\s\t\n\v\h\r]*`_
+ `/*` _`.*`_ `*/`
+ _comment<sub>1</sub>_ _comment<sub>2</sub>_
> A *comment* can appear before and after each token.

### Document
+ `<` _header_ `>`
+ _document_ `@` _declaration_ `;`

### Header
+ `SVDFv0.1`

### Declaration
+ _name_ = _pointer_

### Name
#### Internal names:
+ `!`_`[A-Za-z_]+`_
#### User defined names:
+ _`[A-Za-z_]+`_

### Pointer
+ `(` _pointer_ `)`
+ _pointer<sub>1</sub>_ `&` _pointer<sub>2</sub>_
+ _pointer<sub>1</sub>_ `?` _pointer<sub>2</sub>_
+ _pointer_ * _times_
+ _pointer_ `[` _from_ `:` _to_ `]`
+ _immediate_
+ _name_

### Immediate

#### Single vector
+ `#` _component_count_ _pure_parametric_data_
+ `#` _component_count_ _data_
> The *component_count* token must be a natural (*`[+]?[0-9]+`*) that matches the number of values defined in the *data* token.
#### Vector list
+ `#` *component_count* `*` *vector_count* _data_
> The *component_count* and the _vector_count_ tokens must both be natural (*`[+]?[0-9]+`*) and their product must match the number of values defined in the *data* token.

### Pure parametric data
#### Integer pure parametric data
+ `i`
#### Real pure parametric data
+ `r`

### Data
#### Integer data
+ `i` `:`
+ `i` `:` _integer_array_ _`[,]?`_
#### Real data
+ `r` `:`
+ `r` `:` _real_array_ _`[,]?`_

### Integer array
+ _integer_value_
+ _integer_array_ `,` _integer_value_
> The *integer value* token must be an integer (*`[+-]?[0-9]+`*).
### Real array
+ _real_value_
+ _real_array_ `,` _real_value_
> The *real value* token must be a real (*`[-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?`*).

## Example
```
/* Header */
SVDFv0.1

@ position_comp = 
#3*4r: 
-1.0, -1.0,  0.0,
-1.0,  1.0,  0.0,
 1.0,  1.0,  0.0,
 1.0, -1.0,  0.0;

@ normal_comp
#3*1r: 0.0, 0.0, 1.0;

@ normal_pt = 
normal_comp ? #2i: 1, 1

@ color_rgb = 
#3r;

@ color_a = 
#1r: 1.0;

@ color =
color_rgb & color_a;

@ color_pt =
color * 2

@ inds =
#6i:
1, 2, 3, 3, 4, 5;

@ normal_inds =
#2i:
1, 1;

@ position_tr =

```
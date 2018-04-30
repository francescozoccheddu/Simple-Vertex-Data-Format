# Simple Vertex Data Format
## Data format specification v0.1

### Comment
+ _space_
+ `<` _body_ `>`
+ _comment<sub>1</sub>_ _comment<sub>2</sub>_
> A *comment* can appear before and after each token in a document.  
> The *space* token must match the following regular expression: `[\s\t\n\v\f\r]*`.  
> The *body* token is a string that does not contain the `>` character (`[^>]*`).  

### Document
+ _empty_
+ _document_ _declaration_
> A *document* can be empty.

### Declaration
+ _map_ `;`
+ _map_ `:` _data_  `;` 

### Map
+ _empty_
+ _map_ `?` _key_ `=` `"` _string_ `"`
+ _map_ `#` _key_ `=` _int_
+ _map_ `$` _key_ `=` _float_
> A *map* can be empty.  
> The token *key* is a string with maximum length of 512 characters that matches the following regular expression: `[A-Za-z0-9_.]+`. It must be unique in the map.   
> The token *string* is a string with maximum length of 512 characters that matches the following regular expression `[A-Za-z0-9_.,!?^$%&\/()[\]{}+\-*\\\s]*`.  
> The token *int* is an integer (`[+-]?[0-9]+`).
It can be stored in an **int** C/C++ variable, so its absolute value should not exceed 2<sup>15</sup>.  
> The token *float* is a real number (`[-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?`). It can be stored in a **float** C/C++ variable so its precision may vary.

### Data
+ _real_
+ _data_ , _real_
> The token *real* is a real number (`[-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?`). Its correctness depends on the type used by the parser.

## Example
```
< My 2D quad model >

? model = "2D quad" ;

< Vertex position >
? name = "position"
# count = 4
$ scale = 100.0 
: -1.0, -1.0,  0.0,
  -1.0,  1.0,  0.0, 
   1.0,  1.0,  0.0, 
   1.0, -1.0,  0.0;

< Commented out
? name = "color"
? type = "rgb"
# count = 4
: 1.0, 0.0, 0.0,
  0.0, 0.0, 1.0,
  0.0, 1.0, 0.0,
  1.0, 0.0, 1.0; >

< Vertex color >
? name = "color"
? type = "rgba"
# count = 4
: 1.0, 0.0, 0.0, 1.0,
  0.0, 0.0, 1.0, 1.0,
  0.0, 1.0, 0.0, 1.0,
  1.0, 0.0, 1.0, 1.0;

< Indices >
? name = "indices"
# count = 6
? prim = "triangles"
? type = "short"
: 0, 1, 2,
  2, 3, 4;

```

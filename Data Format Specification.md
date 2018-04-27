# Simple Vertex Data Format
## Data format specification v0.1

### Comment
+ _space_
+ `<` _text_ `>`
+ _comment<sub>1</sub>_ _comment<sub>2</sub>_
> A *comment* can appear before and after each token in a document.  
> The *space* token must match the following regular expression: `[\s\t\n\v\f\r]*`.  
> The *text* token can be any string that does not contain the `>` character (`[^>]*`).  

### Document
+ `[` _header_ `]`
+ _document_ _list_

### Header
+ `SVDFv0.1`

### List
+ `@` _name_ `=` _data_ `;`
> The token *name* cannot be longer than 512 characters and must match the following regular expression: `[0-9A-Za-z_.#$&?]`.  

### Data
+ _empty_
+ _data_ , _real_
> The token *real* must be a real number (`[-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?`).

## Example
```
< Header >
[ SVDFv0.1 ]

@ position = 
-1.0, -1.0,  0.0,
-1.0,  1.0,  0.0,
 1.0,  1.0,  0.0,
 1.0, -1.0,  0.0;

< Commented out, will not be processed
@ color =
1.0, 0.0, 0.0,
0.0, 1.0, 1.0,
0.0, 0.0, 1.0,
0.0, 1.0, 1.0; >

@ index =
1, 2, 3,
3, 4, 5;

```
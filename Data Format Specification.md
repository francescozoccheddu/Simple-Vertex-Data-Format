# C4D<sup>2</sup>C
## Data format specification v0.1

### Empty
+ `?$`

### Natural
+ `[+]?[0-9]+`

### Integer
+ `[+-]?[0-9]+`

### Real
+ `[-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?`

### Whitespace
+ `\s`

### Newline
+ `\n`

### User defined name
+ `[A-Za-z_]+`

### Comment
+ _empty_
+ /* `.\*` */
+ _whitespace_
+ _newline_
+ _comment_ _comment_

Note:  
A _comment_ can be placed between tokens and after the last token.

### File
+ _header_ _whitespace_ _list_section_
+ _header_ _newline_ _list_section_

### Header
+ C4D2Cv0.1

### List section
+ _nothing_
+ _list_section_ _list_

### List
+ @ _data_type # _comp_count_ : _name_ = _data_ `[,]?` ;
+ @ _data_type # _comp_count_ * _vert_count_ : _name_ = _data_ `[,]?` ;
+ : _name_ = _expression_ ;

### Data type
+ ui
+ i
+ f

### Component count
+ _natural_

### Vertex count
+ _natural_

### Name
+ _natural_
+ _user_defined_name_

### Data
+ _natural_
+ _integer_
+ _real_
+ _data_ , _data_

### Expression
TBD

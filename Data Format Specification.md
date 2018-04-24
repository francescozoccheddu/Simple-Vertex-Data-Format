# C4D<sup>2</sup>C
## Data format specification v0.1

> Each symbol preceded or followed by whitespace in a definition is a token.

### Comment
+ _empty_
+ /* _any_string_ */
+ _whitespace_
+ _newline_
+ _comment_ _comment_
> A _comment_ can be placed between each pair of consecutive tokens.

### File
+ < _header_ > _list_section_

### Header
+ C4D2Cv0.1

### List section
+ _empty_
+ _list_section_ _list_

### List
> Single vector:
+ @ _data_type # _comp_count_ : _name_ = _data_ _optional_comma_ ;
> Vector list:
+ @ _data_type # _comp_count_ * _vect_count_ : _name_ = _data_ _optional_comma_ ;
> Derived list:
+ : _name_ = _expression_ ;

### Data type
+ n
+ i
+ f

### Component count
+ _natural_
> The value must match to the number of components of each vector in the _list_.

### Vector count
+ _natural_
> The value must match to the number of vectors in the _list_

### Name
+ _natural_
> Internal usage only. Hidden to the parser.
+ _user_defined_name_
> Visible to the parser.

### Data
+ _empty_
+ _value_
+ _data_ , _data_

### Value
> With __n__ data type:
+ _natural_
> With __i__ data type:
+ _integer_
> With __r__ data type:
+ _real_

### Expression
+ _pointer_
+ _expression_ & _pointer_
> Concatenates multiple *pointer*s to obtain a single interleaved array.
*pointer*s sizes must match, with an exception: if a _pointer_ points to a single vector, then it is automatically repeated.

### Pointer

#### Basic pointer
+ _list_

#### Indexed pointer
+ _basic_pointer<sub>1</sub>_ ? _list<sub>2</sub>_
> Uses the __n__ _list<sub>2</sub>_ as index array for the source _basic_pointer<sub>1</sub>_.

#### Filtered pointer
+ _basic_pointer<sub>1</sub>_ ^ _natural_<sub>2</sub> $ _natural<sub>3</sub>_
+ _indexed_pointer<sub>1</sub>_ ^ _natural_<sub>2</sub> $ _natural<sub>3</sub>_
> Uses the _natural<sub>2</sub>_ as offset for the source _pointer<sub>1</sub>_ and the _natural<sub>3</sub>_ as number of components to retrieve from the same source.

### Regular expression token legend

#### Empty
+ `?$`

#### Natural
+ `[+]?[0-9]+`

#### Integer
+ `[+-]?[0-9]+`

#### Real
+ `[-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?`

#### Whitespace
+ `\s`

#### Newline
+ `\n`

#### User defined name
+ `[A-Za-z_]+`

#### Any string
+ `.*`

#### Optional comma
+ `[,]?`

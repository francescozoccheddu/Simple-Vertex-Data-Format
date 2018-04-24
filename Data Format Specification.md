# Simple Vertex Data Format
## Data format specification v0.1

> Each symbol preceded or followed by whitespace in a definition is a token.

### Comment
+ _empty_
+ /* _any_string_ */
+ _whitespace_
+ _newline_
+ _carriage_return_
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
+ : _name_ # _component_count_ = _data_ _optional_comma_ ;
> Vector list:
+ : _name_ # _component_count_ * _vector_count_ = _data_ _optional_comma_ ;
> Derived list:
+ : _name_ = _expression_ ;

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
+ _natural_
+ _integer_
+ _real_
> The actual expected type depends on the called parse function.

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
> Uses the _list<sub>2</sub>_ with _natural_ data as index array for the source _basic_pointer<sub>1</sub>_.

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

#### Carriage return
+ `\r`

#### User defined name
+ `[A-Za-z_]+`

#### Any string
+ `.*`

#### Optional comma
+ `[,]?`

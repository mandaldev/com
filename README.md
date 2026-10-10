# Com

Bunch of C++ common utils beyond of stdc++, **only for linux**.

Examples can be found at [test](test/) folder.

## Some Important Utils

### Mas

A simple data format file like json but more tiny. See an example:

```mas
{
    "string as key" [ "this" "is" "ok" ]
    key1 [ "special" "key" "can" "avoid" "quoting" ]
    key2 [ "and no comma needed" ]
    "a nested map" {
        1 "value keyed by 1"
    }
    doc "as you can see, '{}' is map, '[]' is array and string are quoted"
}
```

Read doc in [`Value.hpp`](src/Mas/Value.hpp) for detials.

### Mab

Map, Array and Binary -- a binary serialization format, like Mas but serialize to binary.

### Xo

Some command line and process utils.

### Meta

Utils help you do meta-programming.

### Codecs

- [x] Base16
- [x] UTF-8, deocde utf-8 string to/from unicode points (UTF-32)

### Proc

Loaders for Linux proc file system.

### System

Linux system specific utils, like passwd, group loaders.

### ThreadPool

A simple thread pool.

## License

GPLv3

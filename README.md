# COM

bunch of C++ common utils based on stdc++ and enchance stdc++.

## Utils

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

### Xo

Some command line and process utils.

### Meta

Utils help you do meta-programming.

### Codecs

- [x] Base16
- [x] UTF-8, deocde utf-8 string to/from unicode points (UTF-32)

## License

GPLv3

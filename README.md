# butterfly

**butterfly** is an object-oriented 2D graphics library for Lua, targeting the Quartz graphics engine.

## Adding the library to an Xcode project

```sh
make && make install
```

In the Xcode build settings for your target

1. Set **Always Search User Paths** to **Yes**.
2. Add `/usr/local/include` to **User Header Search Paths**.
3. Add `/usr/local/lib/libbutterfly.a` to **Other Linker Flags**.

## lua2png

```sh
make lua2png
./lua2png <width> <height> <input.lua> <output.png>
```

The Lua script returns a function taking a canvas object as its only argument. For example:

```lua
return function(canvas)
    local rect = canvas:metrics():rect()
    local path = Path.point{ x = 0.5, y = 0.7 }
        :addLine{ x = 0.8, y = 1.0 }
        :addLine{ x = 1.0, y = 0.7 }
        :addLine{ x = 0.7, y = 0.4 }
        :addLine{ x = 0.8, y = 0.2 }
        :addLine{ x = 0.6, y = 0.0 }
        :addLine{ x = 0.5, y = 0.2 }
        :addLine{ x = 0.4, y = 0.0 }
        :addLine{ x = 0.2, y = 0.2 }
        :addLine{ x = 0.3, y = 0.4 }
        :addLine{ x = 0.0, y = 0.7 }
        :addLine{ x = 0.2, y = 1.0 }
        :closeSubpath()
    canvas:concatTransformation(Transformation.identity():scale(rect.right - rect.left))
        :setPaint(Color.rgba(1.0, 0.4, 0.3, 1.0))
        :fill(path)
end
```

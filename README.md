# butterfly

**butterfly** is an object-oriented 2D graphics library for Lua, targeting the Quartz graphics engine.

## Adding the library to an Xcode project

1.  **Build and install the library and header files:**
    ```sh
    make && make install
    ```
    You can specify the deployment target using the `MACOSX_DEPLOYMENT_TARGET` environment variable, for example `export MACOSX_DEPLOYMENT_TARGET=10.13`.

2.  **Set the Xcode build settings for your target:**
    - Add `/usr/local/include` to **Header Search Paths** (`HEADER_SEARCH_PATHS`).
    - Add `/usr/local/lib/libbutterfly.a` to **Other Linker Flags** (`OTHER_LDFLAGS`).

3.  **Load butterfly graphics classes in your Lua state:**
    - Add `#include <butterfly/lua.h>` to the file where you initialize your Lua state.
    - Call `bf_lua_load` to set up the globals and metatables for the butterfly graphics classes.

4.  **Create a canvas object for your Lua code to draw into:**
    - Add `#include <butterfly/quartz.h>`.
    - Call `BFCanvasMetricsCreate` and `BFCanvasCreateForDisplay` to create a butterfly canvas from a Quartz graphics context.
    - Call `bf_lua_push` to push the canvas onto the Lua stack. You can use Lua APIs like `lua_setglobal` or `lua_pcall` to assign the canvas to a global variable or call a function passing it as a parameter.

5.  **Draw into the canvas from your Lua scripts.**

## lua2png example

```sh
make lua2png
./lua2png <width> <height> <input.lua> <output.png>
```

The Lua script returns a function taking a canvas object as its only argument. For example:

```lua
return function(canvas)
    local rect = canvas:metrics():rect()
    local path = Path.new()
        :addSubpath{ x = 0.5, y = 0.7 }
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

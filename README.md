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

## Lua classes

The `bf_lua_load` C function installs the following global variables in the Lua state:
  - `Color`
  - `Font`
  - `Gradient`
  - `Icon`
  - `PaintMode`
  - `Path`
  - `StyledString`
  - `Transformation`

### Canvas objects

A canvas represents a Quartz graphics context that can be drawn to. This can be a view or an offscreen image. Unlike the other classes which can be instantiated from Lua scripts, canvases must be provided to Lua from the host environment.

#### Setting canvas attributes

```lua
canvas:setPaint(color)
canvas:setFont(font)
canvas:setOpacity(opacity)
canvas:setThickness(thickness)
canvas:concatTransformation(transformation)
```

#### Drawing paths

```lua
canvas:fill(path)
canvas:stroke(path)
```

#### Drawing text

```lua
canvas:drawText(text, x, y)
canvas:strokeText(text, x, y)
```

### `Color`

#### Getting a color

```lua
local color = Color.rgba(red, green, blue, alpha)
```

`red`, `green`, `blue`, and `alpha` range from 0 to 1.

#### Using a color

```lua
canvas:setPaint(color)
```

Applies the color to subsequent drawing operations in the canvas.

### `Font`

#### Getting a font

```lua
local params = { name = 'Comic Sans MS', size = 18 }
local font = Font.get(params)
```

In addition to the font name and size, the parameters can include the following key-value pairs to request additional font features if they’re supported by the font:

  - `lowercase = 'smallCaps'`
  - `numberCase = 'lowercase'` or `numberCase = 'uppercase'`
  - `numberSpacing = 'proportional'` or `numberSpacing = 'monospaced'`

#### Getting the system font

```lua
local systemFont = Font.system(size)
local boldSystemFont = Font.boldSystem(size)
```

#### Using a font

```lua
canvas:setFont(font)
```

Applies the font to subsequent text drawing operations in the canvas.

Also see _Creating a styled string_.

### `Path`

#### Creating an empty path

```lua
local path = Path.new()
```

#### Adding to a path

```lua
path:addSubpath({ x = x, y = y })
```

Starts a new subpath starting at `x` and `y`.

```lua
path:addLine({ x = x, y = y })
```

Continues from the current point to `x` and `y`.

```lua
path:addCurve({ cx1 = cx1, cy1 = cy1, cx2 = cx2, cy2 = cy2, x = x, y = y })
```

Adds a cubic Bézier curve using the specified control points and ending at `x` and `y`.

```lua
path:closeSubpath()
```

Returns to the starting point of the current subpath and closes the subpath.

```lua
path:addRect({ left = left, bottom = bottom, right = right, top = top }, radius)
```

Adds a new subpath for a rectangle with the specified boundaries. `radius` is optional and if specified will produce a rounded rectangle.

```lua
path:addOval({ left = left, bottom = bottom, right = right, top = top })
```

Adds a new subpath for an oval with the specified boundaries.

#### Drawing a path

A path is not drawn until it’s passed to the canvas `fill` or `stroke` method:

```lua
canvas:fill(path)
canvas:stroke(path)
```

### `StyledString`

#### Creating a styled string

```lua
local params = { font = font }
local styledString = StyledString.new(string, params)
```

#### Combining styled strings

```lua
local styledString = styledString1 .. styledString2
```

#### Drawing a styled string

```lua
canvas:drawText(styledString, x, y)
canvas:strokeText(styledString, x, y)
```

### `Transformation`

#### Creating a null transformation

```lua
local transformation = Transformation.identity()
```

#### Transforming a transformation

```lua
transformation:translate(x, y)
transformation:scale(multiple)
transformation:rotate(radians)
```

#### Combining transformations

```lua
transformation:concat(anotherTransformation)
```

#### Applying a transformation

```lua
canvas:concatTransformation(transformation)
```

Applies the transformation to subsequent drawing operations in the canvas. It’s often a good idea to put this in a `preserve` block so that you can easily return to the original coordinate system:

```lua
canvas:preserve(function(canvas)
    canvas:concatTransformation(transformation)
    -- drawing commands here use the transformed coordinate system
end)
-- drawing commands here use the original coordinate system
```

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

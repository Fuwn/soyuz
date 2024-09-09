> [!NOTE]
> As of [December 19th, 2023](https://www.lunarclient.com/news/how-to-use-the-new-lunar-client-launcher), Lunar Client now has official capacity to disable Discord integration. It only took them nearly 5 years, but it's there.

# 🚀 Soyuz

Soyuz has one simple purpose, which is to listen for incoming Discord RPC requests from Lunar Client and block them!

<a href="https://github.com/fuwn/soyuz">
 <img
   src="https://www.spacesafetymagazine.com/wp-content/uploads/2014/08/early-soyuz-capsule.gif"
   alt="Soyuz"
   width="220">
</a>

## Usage

Prebuilt binaries are located in the
[releases](https://github.com/Fuwn/soyuz/releases/latest).

### Building from source

```powershell
cmake -B build
cmake --build build --config Release
# .\build\Release\soyuz.exe
```

## "Startup" Support

1. Create a shortcut to the Soyuz executable
2. Move the shortcut to
   `C:\Users\<username>\AppData\Roaming\Microsoft\Windows\StartMenu\Programs\Startup`
   
## License

[GNU General Public License v3.0](https://github.com/Fuwn/soyuz/blob/main/LICENSE)

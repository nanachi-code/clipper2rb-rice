# Requirements
## Windows
- Ruby 2.6+ (Installed using RubyInstaller)
- rice 4.x (Installed using RubyGems)

# Build
1. Create make file
```bash
ruby extconf.rb
```

2. Enable MSYS2 commands
```bash
ridk enable
```

3. Build extension
```bash
ridk exec make
```
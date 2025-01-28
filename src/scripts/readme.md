
```lua
print("Local PlayerController : "..string.format("0x%X ", PlayerController.get_local():address()))
print("Local PlayerPawn       : "..string.format("0x%X ", PlayerPawn.get_local():address()))
print("GameEntitySystem       : "..string.format("0x%X ", GameEntitySystem.get():address()))
print("SchemaSystem           : "..string.format("0x%X ", SchemaSystem.get():address()))
```

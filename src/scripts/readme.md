
```lua
print("Local PlayerController : "..string.format("0x%X ", get_local_player():address()))
print("Local PlayerPawn       : "..string.format("0x%X ", get_local_pawn():address()))
print("GameEntitySystem       : "..string.format("0x%X ", GameEntitySystem.get():address()))
print("SchemaSystem           : "..string.format("0x%X ", SchemaSystem.get():address()))
```

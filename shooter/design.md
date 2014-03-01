# Static View
@startuml

hide methods

enum LifeStatus {
 Dormant
 Active
 Destroyed
}

class Animate {
+ location
}
note left
 TODO: Move only moves bouning box; not polygon
end note

Animate -> LifeStatus
game.CollidablePolygon <|-- Animate

class Drome {
+ is static?
}
note left: TODO
Animate <|-- Drome

Animate <|-- Shooter

 
class DromeObject {
+ image-index
+ update and draw
} 
note bottom of DromeObject: <color:red> TODO2

DromeObject --> Drome
game.GameObject <|-- DromeObject

class Shooter {
+ moves
}

class ShooterState {
- encapsulates game state
}
ShooterState *--> Shooter
ShooterState *--> WarZone

class ShooterStateObject {
+ updates game state
+ handles input
}
game.GameObject <|-- ShooterStateObject
ShooterStateObject *-- DromeObject
ShooterState <|-- ShooterStateObject

class WarZone {
- encapsulate map data
- reads BMP and XML
}
WarZone *-- Drome

class ShooterView {
+ renders the state
}
ShooterStateObject <|-- ShooterView


ShooterController *--> ShooterView

@enduml

# Load 

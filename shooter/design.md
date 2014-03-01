# Static View
@startuml
scale 1.2 
hide methods

game.GameObject <|-- game.GameObjectWithParts


enum LifeStatus {
 Dormant
 Active
 Destroyed
} 

class Animate { 
 + location
}


Animate -> LifeStatus
game.CollidablePolygon <|-- Animate

class Drome {
+ is static? 
}

Animate <|-- Drome

Animate <|-- Shooter

 
class DromeObject {
+ image-index
+ update and draw
} 
note left of DromeObject: <color:red> TODO2

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
game.GameObjectWithParts <|-- ShooterStateObject

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

# Load and draw sequence
@startuml
== init ==
main -> ShooterController : construct
main -> Game : construct
ShooterController -> ShooterView : construct
ShooterView -> ShooterStateObject : superclass
ShooterStateObject -> ShooterState : superclass
ShooterState -> Shooter : construct
main -> Game: run
Game -> ShooterController: init
ShooterController -> ShooterView: init
ShooterView -> ShooterStateObject: init
ShooterStateObject ->ShooterState: load map
ShooterState -> WarZone: construct
WarZone -> Drone: construct
ShooterStateObject -> DroneObject: build drones as parts
== update and draw ==
Game -> ShooterController : update
ShooterController -> ShooterStateObject: update 
ShooterView -> DroneObject : update parts
ShooterStateObject -> update : draw
ShooterController -> ShooterView : draw
ShooterView -> DroneObject : draw parts
@enduml

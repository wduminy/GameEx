# Static View
@startuml
scale 1.2 
hide methods


enum LifeStatus {
 Dormant
 Active
 Destroyed
} 

class Animate { 
 + location
}
game.GameObject <|-- Animate


Animate -> LifeStatus
game.CollidablePolygon <|-- Animate

class Drome {
+ is static? 
+ image-index
+ update and draw
}

Animate <|-- Drome

class Shooter {
+ moves
}
Animate <|-- Shooter


class ShooterState {
- dynamic zone objects 
}

ShooterState *--> Shooter
ShooterState *-> WarZone

game.GameObjectWithDynamicParts <|-- ShooterState
ShooterState *-- Drome


class WarZone {
- encapsulate map data (BMP)
}
game.GameObject <|-- WarZone

class ShooterController {
+ handles macro state changes
}

ShooterController *--> ShooterState
game.MainObject <|-- ShooterController
@enduml

# Load and draw sequence
@startuml
scale 1.5 
== init ==
main -> ShooterController : construct
ShooterController ->  ShooterState : construct
ShooterState -> Shooter : construct
main -> Game : construct and run
Game -> ShooterController: init
ShooterController -> ShooterState: init
ShooterState -> WarZone: construct
ShooterState -> Drone: build drones as parts
== update and draw ==
Game -> ShooterController : update
ShooterController -> ShooterState: update 
ShooterState -> Drone : update parts
ShooterController -> ShooterState : draw
ShooterState -> Drone : draw parts
@enduml

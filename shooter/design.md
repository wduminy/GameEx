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
note left: <color:red> TODO
Animate -> LifeStatus

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
GameObject <|-- DromeObject

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
GameObject <|-- ShooterStateObject
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

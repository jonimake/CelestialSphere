# Celestial Sphere Plugin

A plugin for creating a realistically rotating celestial sphere with stars around the game world in Unreal Engine 4.
[A small demo video on youtube](https://www.youtube.com/watch?v=coOf1ppWQe8).
## Features

The plugin consists of two main classes. The first is an actor component that rotates the owning actor as sidereal time advances. 
The second one is an actor that creates a star sphere around the actor and rotates itself using the previously mentioned rotation component.

### CelestialRotationComponent

This component rotates the owning actor by calculating the local sidereal angle for the given latitude, longitude and time.
The rotation rate is tied to time and it can be increased by increasing the `TimeScale` property.

### ProceduralCelestialSphere

This actor offers the ability to generate a star sphere procedurally around this actor. 

The star geometry itself is either a simple triangle or a quad, this can be changed by changing the `Geometry` property of the actor.
The generation process stores the star's magnitude and colour temperature in Kelvins in a secondary UV map of the mesh. This data
can in turn be used in the material of the mesh.

Stars' data can be found under the resources folder. 
Source of this data is [https://github.com/astronexus/HYG-Database](https://github.com/astronexus/HYG-Database).
You can obviously use any kind of data, as long as it can be massaged into the `FStar` struct.

### Star data

The star coordinates are in J2000.0 epoch and equinox. 

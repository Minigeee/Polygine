# Polygine

A custom game engine developed as a personal project. This engine was originally designed for a 3D low-poly art style, hence the name "Polygine" but has expanded to include standard 3D graphics. This was built to explore and understand the various components that make up a game engine, including audio, graphics, physics, and more.

*Please note that this engine is incomplete and is not intended for actual production use.*

## Features

All source files for the following modules can be found [here](https://github.com/Minigeee/Polygine/tree/master/include/poly).

All classes are documented with descriptions and usage examples, and documentation can be generated using Doxygen.

### Audio
This module provides support for audio playback, recording, and manipulation. It includes features for managing audio buffers, streaming audio, handling different sound formats, and implementing 3D audio effects with listener support. It is mostly a wrapper around SFML classes.

### Core
The Core module provides common core functionality such as memory management, logging, timekeeping, and task scheduling. It also includes utilities for handling different data types and arrays, as well as support for XML parsing.

### Engine
This engine uses the Entity-Component-System (ECS) architecture, which allows for simpler game object management, while maintaining cache-friendly data access patterns. The Engine module manages entities, their components, and the events that affect them, allowing for the creation of complex game scenes. Input handling is also handled under this module.

### Graphics
This module handles all rendering operations within the engine, including 3D model management, shader handling, deferred lighting, and post-processing effects. It also supports skeletal animations and particle systems, as well as specialized systems for handling materials and textures. The rendering framework is built mostly as an OpenGL abstraction layer, with a more complex, general purpose rendering pipeline structure on top. It is designed to be extendable to allow for more advanced graphical features.

### Math
The Math module provides the mathematical common math utility, including vector and matrix operations, transformations, and quaternions. This module is used to handle spatial calculations and other mathematical operations needed throughout the engine.

### Network
The Network module enables basic networking capabilities, including IP address management, TCP/UDP socket handling, and data packet management. This allows for the development of networked applications or games, supporting both client-server and peer-to-peer architectures. It is mostly a wrapper around SFML classes.

### Physics
This module includes a basic physics engine with support for various collider types and collision detection. It enables the simulation of physical interactions between objects in a 3D environment, including handling of joints and physical constraints. ReactPhysics3D is used as the internal physics engine.

### UI
The UI module provides a set of tools for creating and managing user interface elements, including buttons, sliders, and text inputs. It supports layout loading, event handling, and rendering of UI components.


## Demo

View a video demo of an app created with this engine [here](https://drive.google.com/file/d/1EHWQczH3o2_sGxZSTpOrJcyZuKZ-Qtfj/view?usp=sharing).

[![Link to demo app video](/examples/demo.png "Demo App")](https://drive.google.com/file/d/1EHWQczH3o2_sGxZSTpOrJcyZuKZ-Qtfj/view?usp=sharing)

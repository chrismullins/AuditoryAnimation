Auditory Animation
=================

Overview
-------

### UNC Graphics Project
 * The goal of this project is to be able to visualize music in a customizeable format.
 * A scene will be generated based on a config file selected by the user. 
 * Scene objects will be modified (rotated and/or resized) based on different audio frequency bands.

Program Workflow
---------------

### Controller
 * Initializes a model based on the config file (takes data from a ConfigFileReader)
 * Starts the sound processing (sends data to Sound Processor)
 * Builds/Updates the scene (sends and receives data to the Model)
 * Renders the scene (sends data to the View)

 * Output video 

Sends data to the Model and the view

### Model
 * Maintains the state of the scene (receives data from Controller)
 * Getters/Setters
 * Adders/Removers

### View
 * Renders the scene by Model (receives data from Controller)
 * Static, no dependencies aside from the model

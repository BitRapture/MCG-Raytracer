#include "MCG_Raytracing.h"

// Scene Manager
namespace MRT
{
	void SceneManager::FillInstructionList()
	{
		// Setting it like this allows for future instructions to be made
		instructionSet = 12;
		instructions = new std::string[]{
			"exit", "help", "uioff", "render",
			"clear", "color", "move", "rotate",
			"lookat", "fov", "circle", "sphere"
		};
	}

	void SceneManager::InstHelp()
	{
		std::cout << "Help page (rev0):\n\n" <<
			"Contents are ordered as followed: [instruction name] : (parameters) : {description}\n\n" <<
			"exit : \n n/a \n: Exit the application\n\n" << 
			"help : \n n/a \n: Displays the help page\n\n" <<
			"render : \n n/a \n: Render the scene through software raytracing\n\n" <<
			"uioff : \n n/a \n: Turns the user input off, allows for the graphic window to be moved\n\n" <<
			"clear : \n n/a \n: Clears all objects on the current scene\n\n" <<
			"color : \n [bgColor]: float:Red(0 to 1) float:Green(0 to 1) float:Blue(0 to 1) \n: Change the background color of the scene\n\n" <<
			"move : \n [camPosition]: float:X float:Y float:Z \n: Set the camera position in the world\n\n" <<
			"rotate : \n [camRotation]: float:axisX float:axisY float:axisZ \n [amountOfRotation]: float:angleDegrees \n: Set the camera rotation matrix via an axis vecor\n\n" <<
			"lookat : \n [camTarget]: float:X float:Y float:Z \n: Set the camera rotation matrix to look at a target point\n\n" <<
			"fov : \n [camFOV]: float:angleDegrees \n: Set the camera FOV in degrees\n\n" <<
			"circle : \n [circlePosition]: float:X float:Y float:Z \n [circleDireciton]: float:axisX float:axisY float:axisZ \n " << 
				"[circleRadius]: float:Radius \n [circleColor]: float:Red(0 to 1) float:Green(0 to 1) float:Blue(0 to 1) \n: " <<
					"Add a 2D circle object to the scene\n\n" <<
			"sphere : \n [spherePosition]: float:X float:Y float:Z \n [sphereRadius]: float:Radius \n [sphereColor]: float:Red(0 to 1) float:Green(0 to 1) float:Blue(0 to 1) \n: " <<
				"Add a sphere object to the scene\n\n" 
			<< std::endl;
	}

	void SceneManager::InstUioff()
	{
		fUserEnabled = false;
		std::cout << "User input has been disabled, to renable please reopen the application.\n" << std::endl;
	}

	void SceneManager::InstRender()
	{
		std::cout << "Rendering scene, please wait..." << std::endl;
		raytracer->RenderScene();
		std::cout << "Scene successfully rendered.\n" << std::endl;
	}

	void SceneManager::InstClear()
	{
		raytracer->ClearPrimitives();
		std::cout << "Cleared scene.\n" << std::endl;
	}

	bool SceneManager::InstColor(std::string* _argv, int& _argc)
	{
		// color float:R float:G float:B
		if (_argc != 4) return false;
		float r = std::strtof(_argv[1].c_str(), NULL), g = std::strtof(_argv[2].c_str(), NULL),
			b = std::strtof(_argv[3].c_str(), NULL);

		raytracer->SetBackgroundColor({ r, g, b });
		std::cout << "Scene background color set to: {" << r << ", " << g << ", " << b << "}.\n" << std::endl;
		return true;
	}

	bool SceneManager::InstMove(std::string* _argv, int& _argc)
	{
		// move float:X float:Y float:Z
		if (_argc != 4) return false;
		float x = std::strtof(_argv[1].c_str(), NULL), y = std::strtof(_argv[2].c_str(), NULL),
			z = std::strtof(_argv[3].c_str(), NULL);

		raytracer->SetCameraPosition({ x, y, z });
		std::cout << "Camera position set to: {" << x << ", " << y << ", " << z << "}.\n" << std::endl;
		return true;
	}

	bool SceneManager::InstRotate(std::string* _argv, int& _argc)
	{
		// rotate float:X float:Y float:Z float:Degrees
		if (_argc != 5) return false;
		float x = std::strtof(_argv[1].c_str(), NULL), y = std::strtof(_argv[2].c_str(), NULL),
			z = std::strtof(_argv[3].c_str(), NULL), degrees = std::strtof(_argv[4].c_str(), NULL);

		raytracer->SetCameraRotation({ x, y, z }, degrees);
		std::cout << "Camera rotation set to: " << degrees << 
			" degrees around " << "{" << x << ", " << y << ", " << z << "} axis.\n" << std::endl;
		return true;
	}

	bool SceneManager::InstLookat(std::string* _argv, int& _argc)
	{
		// lookat float:X float:Y float:Z
		if (_argc != 4) return false;
		float x = std::strtof(_argv[1].c_str(), NULL), y = std::strtof(_argv[2].c_str(), NULL),
			z = std::strtof(_argv[3].c_str(), NULL);

		raytracer->SetCameraTarget({ x, y, z });
		std::cout << "Camera rotation set to: " << "{" << x << ", " << y << ", " << z << "} target position.\n" << std::endl;
		return true;
	}

	bool SceneManager::InstFOV(std::string* _argv, int& _argc)
	{
		// fov float:Degrees
		if (_argc != 2) return false;
		float degrees = std::strtof(_argv[1].c_str(), NULL);

		raytracer->SetCameraFOV(degrees);
		std::cout << "Camera FOV set to: " << degrees << " degrees.\n" << std::endl;
		return true;
	}

	bool SceneManager::InstCircle(std::string* _argv, int& _argc)
	{
		// circle float:X float:Y float:Z float:FaceX float:FaceY float:FaceZ float:Radius
		// float:R float:G float:B
		if (_argc != 11) return false;
		float x = std::strtof(_argv[1].c_str(), NULL), y = std::strtof(_argv[2].c_str(), NULL),
			z = std::strtof(_argv[3].c_str(), NULL), faceX = std::strtof(_argv[4].c_str(), NULL),
			faceY = std::strtof(_argv[5].c_str(), NULL), faceZ = std::strtof(_argv[6].c_str(), NULL),
			radius = std::strtof(_argv[7].c_str(), NULL), r = std::strtof(_argv[8].c_str(), NULL), 
			g = std::strtof(_argv[9].c_str(), NULL), b = std::strtof(_argv[10].c_str(), NULL);

		MRT::Circle* circle = new MRT::Circle({ x, y, z }, { faceX, faceY, faceZ }, radius, { r, g, b });
		if (circle == nullptr) return false;

		raytracer->AddPrimitive(circle);
		std::cout << "Added circle to scene at position: " << "{" << x << ", " << y << ", " << z << "}.\n With a radius set to: " <<
			radius << ".\n Facing direction: " << "{" << x << ", " << y << ", " << z <<
			"}.\n And a color set to: " << "{" << r << ", " << g << ", " << b << "}.\n" << std::endl;
		return true;
	}

	bool SceneManager::InstSphere(std::string* _argv, int& _argc)
	{
		// sphere float:X float:Y float:Z float:Radius float:R float:G float:B
		if (_argc != 8) return false;
		float x = std::strtof(_argv[1].c_str(), NULL), y = std::strtof(_argv[2].c_str(), NULL),
			z = std::strtof(_argv[3].c_str(), NULL), radius = std::strtof(_argv[4].c_str(), NULL),
			r = std::strtof(_argv[5].c_str(), NULL), g = std::strtof(_argv[6].c_str(), NULL),
			b = std::strtof(_argv[7].c_str(), NULL);

		MRT::Sphere* sphere = new MRT::Sphere({ x, y, z }, radius, { r, g, b });
		if (sphere == nullptr) return false;

		raytracer->AddPrimitive(sphere);
		std::cout << "Added sphere to scene at position: " << "{" << x << ", " << y << ", " << z << "}.\n With a radius set to: " <<
			radius << ".\n And a color set to: " << "{" << r << ", " << g << ", " << b << "}.\n" << std::endl;
		return true;
	}

	void SceneManager::Run()
	{
		if (!fInitialised) return;
		
		// Application start info
		std::cout << "MCG RayTracer, created by George Smith (2021). \nTo get started, type " <<
			"'help' for a full view of all usable instructions (They're case sensitive!). \nThere's a scene already setup, type 'render' to raytrace it. " <<
			" \nClose the program by typing 'exit'.\n" << std::endl;

		// Declare user input and arguments
		std::string input, * argv;
		while (MCG::ProcessFrame())
		{
			if (fUserEnabled)
			{
				std::cout << "raytracer > ";
				// Get user input
				std::getline(std::cin, input);

				input = input.substr(0, input.find_last_of("abcdefghijklmnopqrstuvwxyz0123456789.") + 1);

				// Get the total amount of arguments
				int argc = 1;
				for (std::string::iterator i = input.begin(); i != input.end(); ++i)
					if ((*i) <= 0x20) ++argc;

				// Create new array for arguments
				argv = new std::string[argc];
				if (argv == nullptr) return;
				// Split the input string up into args
				int carg = 0;
				for (std::string::iterator i = input.begin(); i != input.end(); ++i)
				{
					if ((*i) <= 0x20)
					{
						++carg;
						continue;
					}
					argv[carg] += (*i);
				}

				// Query the first argument with instruction list
				bool instUsed = false, instRan = true;
				for (int inst = 0; inst < instructionSet; ++inst)
				{
					// Can safely assume argv[0] is allocated since argc is 1 by default
					if (instructions[inst].compare(argv[0]) == 0)
					{
						switch (inst)
						{
							  // exit
						case 0: { return;  break; }
							  // help
						case 1: { InstHelp(); break; }
							  // uiof
						case 2: { InstUioff(); break; }
							  // render
						case 3: { InstRender();  break; }
							  // clear
						case 4: { InstClear(); break; }
							  // color
						case 5: { instRan = InstColor(argv, argc); break; }
							  // move
						case 6: { instRan = InstMove(argv, argc); break; }
							  // rotate
						case 7: { instRan = InstRotate(argv, argc); break; }
							  // lookat
						case 8: { instRan = InstLookat(argv, argc); break; }
							  // fov
						case 9: { instRan = InstFOV(argv, argc); break; }
							  // circle
						case 10: { instRan = InstCircle(argv, argc); break; }
							  // sphere
						case 11: { instRan = InstSphere(argv, argc); break; }
						}
						instUsed = true;
						break;
					}
				}
				// Generic errors to console if failed to run instructions
				if (!instUsed && argv[0].length() > 0)
				{
					std::cout << "'" << argv[0] << "' is an unknown instruction, type 'help' to view all usable instructions.\n" << std::endl;
				}
				if (instUsed && !instRan)
				{
					std::cout << "'" << argv[0] << "' instruction was malformed, type 'help' to check the parameters are correct.\n" << std::endl;
				}

				// Delete arguments
				delete[] argv;
				argv = nullptr;
			}
		}
	}

	SceneManager::SceneManager(RayTracer* _raytracer)
		:
		raytracer{ _raytracer }
	{
		// Double check that the raytracer is already instantiated
		fInitialised = (raytracer != nullptr ? raytracer->IsInit() : false);

		// Fill instruction list and make sure its been allocated
		FillInstructionList();
		fInitialised &= (instructions != nullptr);
	}
}
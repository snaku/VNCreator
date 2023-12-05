# VNCreator

VNCreator est un moteur de jeu simple conçu pour accélérer le processus de création de Visual Novels. 

# Caractéristiques de VNCreator

* Rendu graphique Vulkan
* Gestion de la Fenêtre avec GLFW ou SDL2 (selon votre choix !).
* Son avec OpenAL (En développement)

## État actuel

**VNCreator est actuellement en développement. Très peu de fonctionnalités sont présentes (2 fonctions...) mais ce moteur est régulièrement mis à jour !**

# Template
Pour vous aidez, voici un code de base sur l'initialisation de VNCreator avec GLFW:

```cpp
#include "Window.h" // La class de la fenêtre GLFW

#define VNCREATOR_USING_GLFW /*macro permettant d'utiliser la fonction VNCreator::start(),
							   mais avec la gestion de la fenêtre GLFW*/

//TOUJOURS mettre les macros APRES l'inclusion de GLFW et Vulkan et AVANT VNCreator.h

#include "VNCreator.h" // Inclusion de VNCreator

#include <stdexcept> // std::exception
#include <iostream> // std::cerr

int main()
{
	try
	{
		Window window; // Votre objet Window GLFW

		// à noter qu'il faut ABSOLUMENT mettre en dernier la fonction VNCreator::start() dans le bloc try

		// Votre code ici

		/* VNCreator::start prend en paramètre un objet de type GLFWwindow*. Dans notre cas,
		   nous utiliserons un getter*/
		VNCreator::start(window.getWindow()); 
	}
	// Les exceptions
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}
```

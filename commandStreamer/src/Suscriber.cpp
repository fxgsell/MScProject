
#include "Suscriber.h"

template<typename T>
Suscriber<T>::Suscriber(const char *base_name,
			int argc, char **argv) 

template<typename T>
void Suscriber<T>::init(const char *topic, void (*cb)(const ros::MessageEvent<T>&))

template<typename T>
Suscriber<T>::~Suscriber() {
}

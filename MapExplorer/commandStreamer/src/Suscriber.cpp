
#include "Suscriber.h"

template<typename T>
Suscriber<T>::Suscriber(const char* topic, void (*cb)(const ros::MessageEvent<T>&)) {} 

template<typename T>
Suscriber<T>::~Suscriber() {
}

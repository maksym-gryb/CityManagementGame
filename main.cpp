#include "app.h"

int main()
{
	return App::getInstance()->run(new MainScene);
}

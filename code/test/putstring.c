#ifdef CHANGED

#include "syscall.h"

void print(char *c)
{
#if 1
	PutString(c);
	PutChar('\n');
#endif
}

int main()
{
	print("ceci est un test de putstring avec un lonnnnnggggggg string! \n \
	Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed non \n \
	risus. Suspendisse lectus tortor, dignissim sit amet, adipiscing \n \
	nec, ultricies sed, dolor. Cras elementum ultrices diam. Maecenas \n \
	ligula massa, varius a, semper congue, euismod non, mi. Proin \n \
	porttitor, orci nec nonummy molestie, enim est eleifend mi, non \n \
	fermentum diam nisl sit amet erat. Duis semper. Duis arcu massa, \n \
	scelerisque vitae, consequat in, pretium a, enim. Pellentesque \n \
	congue. Ut in risus volutpat libero pharetra tempor. Cras vestibulum \n \
	bibendum augue. Praesent egestas leo in pede. Praesent blandit odio \n \
	eu enim. Pellentesque sed dui ut augue blandit sodales. Vestibulum \n \
	ante ipsum primis in faucibus orci luctus et ultrices posuere \n \
	cubilia Curae; Aliquam nibh. Mauris ac mauris sed pede pellentesque \n \
	fermentum. Maecenas adipiscing ante non diam sodales hendrerit. \n \
	\n \
	Ut velit mauris, egestas sed, gravida nec, ornare ut, mi. Aenean ut \n \
	orci vel massa suscipit pulvinar. Nulla sollicitudin. Fusce varius, \n \
	ligula non tempus aliquam, nunc turpis ullamcorper nibh, in tempus \n \
	sapien eros vitae ligula. Pellentesque rhoncus nunc et augue. \n \
	Integer id felis. Curabitur aliquet pellentesque diam. Integer quis \n \
	metus vitae elit lobortis egestas. Lorem ipsum dolor sit amet, \n \
	consectetuer adipiscing elit. Morbi vel erat non mauris convallis \n \
	vehicula. Nulla et sapien. Integer tortor tellus, aliquam faucibus, \n \
	convallis id, congue eu, quam. Mauris ullamcorper felis vitae erat. \n \
	Proin feugiat, augue non elementum posuere, metus purus iaculis \n \
	lectus, et tristique ligula justo vitae magna. \n \
	\n \
	Aliquam convallis sollicitudin purus. Praesent aliquam, enim at \n \
	fermentum mollis, ligula massa adipiscing nisl, ac euismod nibh nisl \n \
	eu lectus. Fusce vulputate sem at sapien. Vivamus leo. Aliquam \n \
	euismod libero eu enim. Nulla nec felis sed leo placerat imperdiet. \n \
	Aenean suscipit nulla in justo. Suspendisse cursus rutrum augue. \n \
	Nulla tincidunt tincidunt mi. Curabitur iaculis, lorem vel rhoncus \n \
	faucibus, felis magna fermentum augue, et ultricies lacus lorem \n \
	varius purus. Curabitur eu amet.");
	print("Test avec des caractères spéciaux \n\t²&é~\"#'{([-|è`_ç^à@)]=}°+¨£$¤%ùµ*;.:/!§ \\n");
	print("petite chaine");
	print("Chaine moyenne:dsds");
	print("Chaine moyenne:dsdss");
	print("Chaine moyenne:dsdsss");
	return 0;
}

#endif // CHANGED

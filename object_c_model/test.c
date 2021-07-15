#include <stdio.h>
#include <string.h>

struct object {
	char name[10];
};

struct device {
	struct object obj;
	int is_hotplug;
};

struct pci_device {
	struct device dev;
	int device_id;
	int vendor_id;
};

int main()
{
	struct pci_device *pdev;
	struct device *dev;
	struct object *obj;

	/* 静态生成的类 */
	struct pci_device pci_dev;
	pdev = &pci_dev;
	dev = (struct device *)&pci_dev;
	obj = (struct object *)&pci_dev;

	memcpy(obj->name, "pci1", strlen("pci1") + 1);
	printf("--> name of pci device: %s\n", ((struct object *)pdev)->name);

	/* 动态生成的类, 需要生成类大小信息，或者直接知道定义，或者有一个类大小描述 */
	pdev = calloc(1, sizeof(*pdev));
	if (!pdev)
		exit(-1);

	return 0;
}

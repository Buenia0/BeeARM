#include "../BeeARM/beearm.h"
#include "../BeeARM/beearm_tables.h"
#include <string>
#include <fstream>
using namespace beearm;
using namespace std;

class TestInterface : public BeeARMInterface
{
  public:
    TestInterface();
    ~TestInterface();

    array<uint8_t, 0x10000000> memory;

    void loadfile(string filename)
    {
	ifstream file(filename.c_str(), ios::in | ios::binary | ios::ate);

	if (file.is_open())
	{
	    streampos size = file.tellg();
	    file.seekg(0, ios::beg);
	    file.read((char*)&memory[0x8000000], size);
	    cout << "Success" << endl;
	    file.close();
	}
	else
	{
	    cout << "Error" << endl;
	    exit(1);
	}
    }

    uint8_t readByte(uint32_t addr)
    {
      addr = (addr % 0x10000000);
      return memory[addr];
    }

    void writeByte(uint32_t addr, uint8_t val)
    {
	memory[addr] = val;
    }

    uint16_t readWord(uint32_t addr)
    {
      return *(uint16_t*)&memory[addr];
      // return ((readByte(addr + 1) << 8) | (readByte(addr)));
    }

    void writeWord(uint32_t addr, uint16_t val)
    {
	writeByte(addr, (val & 0xFF));
	writeByte((addr + 1), (val >> 8));
    }

    uint32_t readLong(uint32_t addr)
    {
      return *(uint32_t*)&memory[addr];
      // return ((readWord(addr + 2) << 16) | (readWord(addr)));
    }

    void writeLong(uint32_t addr, uint32_t val)
    {
	writeWord(addr, (val & 0xFFFF));
	writeWord((addr + 2), (val >> 16));
    }

    int clockcycle(uint32_t val, int flags)
    {
	return 1;
    }

    void update()
    {
	return;
    }

    void softwareinterrupt(uint32_t val)
    {
	return;
    }

    int getversion()
    {
	return 4;
    }

    uint32_t readcoprocessor(uint16_t id)
    {
	return 0;
    }

    void writecoprocessor(uint16_t id, uint32_t val)
    {
	return;
    }
    
    void exceptionreturncallback()
    {
        return;
    }
};

TestInterface::TestInterface()
{

}

TestInterface::~TestInterface()
{

}

TestInterface inter;
BeeARM arm;

void init(string filename)
{
    inter.loadfile(filename);
    arm.setinterface(&inter);
    arm.init(0x0, 0x5F);
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
	cout << "Usage: " << argv[0] << " FILE" << endl;
	return 1;
    }

    init(argv[1]);

    for (int i = 0; i < 10000000; i++)
    {
	arm.executenextinstr();
    }

    cout << "Program execution finished." << endl;
    return 0;
}

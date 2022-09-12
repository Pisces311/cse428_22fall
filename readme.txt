lab0
Ke Peng k.peng@wustl.edu
Zichao Liang liang.zichao@wustl.edu
Siqian Hou h.siqian@wustl.edu

1. Warning fixes: When we first compiled this program, there was a warning: destructor called on non-final 'HoldEmDeck' that has virtual functions but non-virtual destructor.
After searching the internet, we learned a C++ Core guideline: A base class destructor should be either public and virtual, or protected and nonvirtual. So we finally added a virtual destructor to the 'Deck' class.
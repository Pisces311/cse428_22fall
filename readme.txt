lab0
Ke Peng k.peng@wustl.edu
Zichao Liang liang.zichao@wustl.edu
Siqian Hou h.siqian@wustl.edu

1. Warning fixes: When we first compiled this program, there was a warning: destructor called on non-final 'HoldEmDeck' that has virtual functions but non-virtual destructor.
After searching the internet, we learned a C++ Core guideline: A base class destructor should be either public and virtual, or protected and nonvirtual. So we finally added a virtual destructor to the 'Deck' class.

2. Error fix: We got 1 error when we compiled and ran our program at the first time, the error is as below:

/tmp/ccXAT4CI.o: In function \`PinochleDeck::print(std::ostream&)‘:
/home/warehouse/k.peng/My Documents/cse428\_fall22/lab0/PinochleDeck.cpp:21: undefined reference to \`std::ostream& operator<< <PinochleRank, Suit>(std::ostream&, Card<PinochleRank, Suit> const&)’
/tmp/ccWZnjfG.o: In function \`HoldEmDeck::print(std::ostream&)‘:
/home/warehouse/k.peng/My Documents/cse428\_fall22/lab0/HoldEmDeck.cpp:19: undefined reference to \`std::ostream& operator<< <HoldEmRank, Suit>(std::ostream&, Card<HoldEmRank, Suit> const&)’

We thought this was because the overwritten operator << in Card_T.h and Card_T.cpp was not referred properly. We checked
the textbook and searched from the internet then found that the cause should be the property of template that when using
template, we should include the corresponding .h and .cpp file at the same time. We have two solutions at the first time:

1. Directly implement << in Card_T.h file. This approach works but not fit the lab instruction which ask us to separate declaration
and definition. So we discard this approach.

2. include both Card_T.h and Card_T.cpp in lab0.cpp. This approach also works but is not elegant since if we need to use
Card_T in the future, we still need to include both of them. So we also discard this approach.

Finally, with the help of professor, we decide to use conditional inclusion guard which add the following lines at the end of
Card_T.h file:

#ifdef TEMPLATE_HEADERS_INCLUDE_SOURCE
#include "Card_T.cpp"
#endif

This part of code helps include .cpp along with .h so this approach solve the problem while separate declaration and definition
at the same time.
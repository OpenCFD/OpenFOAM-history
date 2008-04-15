/*
 * This file is part of the load-balancing example
 *
 * This is the header file of the printer servant.
 */

#include "Printer.h"


class Printer_impl : virtual public POA_Printer
{
  public :
    Printer_impl (const char *printerName);
    void print (const char *message);
    ~Printer_impl ();

  private :
    char *printerName;
};


/*
 * This file is part of the load-balancing example
 *
 * This is the implementation of the printer servant.
 */

#include "Printer_impl.h"


Printer_impl::Printer_impl (const char *name)
{
  if (name == NULL)
    this->printerName = strdup ("DefaultName");
  else
    this->printerName = strdup (name);

  printf ("++ printer %s is created\n", this->printerName);
}


void Printer_impl::print (const char *message)
{
  printf ("** printer %s prints [%s]\n", this->printerName, message);
}


Printer_impl::~Printer_impl ()
{
  printf ("-- printer %s is destroyed\n");

  if (this->printerName != NULL)
    free (this->printerName);
}

/*=========================================================================

  Program:   ParaView
  Module:    $RCSfile: vtkPVFoamSelectTimeSet.cxx,v $

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkPVFoamSelectTimeSet.h"

#include "vtkDataArrayCollection.h"
#include "vtkFloatArray.h"
#include "vtkKWFrame.h"
#include "vtkKWLabel.h"
#include "vtkKWLabeledFrame.h"
#include "vtkKWMenu.h"
#include "vtkObjectFactory.h"
#include "vtkPVAnimationInterfaceEntry.h"
#include "vtkPVApplication.h"
#include "vtkPVProcessModule.h"
#include "vtkPVScalarListWidgetProperty.h"
#include "vtkPVSource.h"
#include "vtkPVXMLElement.h"

#include <vtkstd/string>

//-----------------------------------------------------------------------------
vtkStandardNewMacro(vtkPVFoamSelectTimeSet);
vtkCxxRevisionMacro(vtkPVFoamSelectTimeSet, "$Revision: 1.39 $");

//-----------------------------------------------------------------------------
int vtkDataArrayCollectionCommand(ClientData cd, Tcl_Interp *interp,
                                  int argc, char *argv[]);

//-----------------------------------------------------------------------------
vtkPVFoamSelectTimeSet::vtkPVFoamSelectTimeSet()
{
  this->LabeledFrame = vtkKWLabeledFrame::New();
  this->LabeledFrame->SetParent(this);
  
  this->TimeLabel = vtkKWLabel::New();
  this->TimeLabel->SetParent(this->LabeledFrame->GetFrame());

  this->TreeFrame = vtkKWWidget::New();
  this->TreeFrame->SetParent(this->LabeledFrame->GetFrame());

  this->Tree = vtkKWWidget::New();
  this->Tree->SetParent(this->TreeFrame);

  this->TimeValue = 0.0;

  this->FrameLabel = 0;
  
  this->TimeSets = vtkDataArrayCollection::New();
  
  this->Property = 0;
  
  this->SetCommand = 0;
  this->ServerSideID.ID = 0;
}

//-----------------------------------------------------------------------------
vtkPVFoamSelectTimeSet::~vtkPVFoamSelectTimeSet()
{
  this->LabeledFrame->Delete();
  this->Tree->Delete();
  this->TreeFrame->Delete();
  this->TimeLabel->Delete();
  this->SetFrameLabel(0);
  this->TimeSets->Delete();
  this->SetSetCommand(0);
  if(this->ServerSideID.ID)
    {
    vtkPVProcessModule* pm = this->GetPVApplication()->GetProcessModule();
    pm->DeleteStreamObject(this->ServerSideID);
    pm->SendStream(vtkProcessModule::DATA_SERVER);
    }
}

//-----------------------------------------------------------------------------
void vtkPVFoamSelectTimeSet::SetLabel(const char* label)
{
  this->SetFrameLabel(label);
  if (this->GetApplication())
    {
    this->LabeledFrame->SetLabel(label);
    }
}

//-----------------------------------------------------------------------------
const char* vtkPVFoamSelectTimeSet::GetLabel()
{
  return this->GetFrameLabel();
}

//-----------------------------------------------------------------------------
void vtkPVFoamSelectTimeSet::Create(vtkKWApplication *pvApp)
{
  // Call the superclass to create the widget and set the appropriate flags

  if (!this->vtkKWWidget::Create(pvApp, "frame", "-bd 2 -relief flat"))
    {
    vtkErrorMacro("Failed creating widget " << this->GetClassName());
    return;
    }

  // For getting the widget in a script.
  if ((this->TraceNameState == vtkPVWidget::Uninitialized ||
       this->TraceNameState == vtkPVWidget::Default) )
    {
    this->SetTraceName("FoamSelectTimeSet");
    this->SetTraceNameState(vtkPVWidget::SelfInitialized);
    }
  
  this->LabeledFrame->Create(this->GetApplication(), 0);
  if (this->FrameLabel)
    {
    this->LabeledFrame->SetLabel(this->FrameLabel);
    }
  this->TimeLabel->Create(this->GetApplication(), "");

  char label[32];
  sprintf(label, "Time value: %12.5e", 0.0);
  this->TimeLabel->SetLabel(label);
  this->Script("pack %s", this->TimeLabel->GetWidgetName());
  
  this->TreeFrame->Create(this->GetApplication(), "ScrolledWindow", 
                          "-relief sunken -bd 2");

  this->Tree->Create(this->GetApplication(), "Tree", 
                     "-background white -bd 0 -width 15 -padx 2 "
                     "-redraw 1 -relief flat -selectbackground red");
  this->Script("%s bindText <ButtonPress-1>  {%s SetTimeValueCallback}",
               this->Tree->GetWidgetName(), this->GetTclName());
  this->Script("%s setwidget %s", this->TreeFrame->GetWidgetName(),
               this->Tree->GetWidgetName());

  this->Script("pack %s -expand t -fill x", this->TreeFrame->GetWidgetName());

  this->Script("pack %s -side top -expand t -fill x", 
               this->LabeledFrame->GetWidgetName());

}

//-----------------------------------------------------------------------------
void vtkPVFoamSelectTimeSet::SetTimeValue(float time)
{
  if (this->TimeValue != time ||
      !this->TimeLabel->GetLabel() ||
      !strcmp(this->TimeLabel->GetLabel(), "No timesets available."))
    { 
    this->TimeValue = time; 
    
    char label[32];
    sprintf(label, "Time value: %12.5e", time);
    this->TimeLabel->SetLabel(label);
    this->Modified(); 
    } 
}

//-----------------------------------------------------------------------------
void vtkPVFoamSelectTimeSet::SetTimeValueCallback(const char* item)
{
  if (this->TimeSets->GetNumberOfItems() == 0)
    {
    return;
    }

  if ( strncmp(item, "timeset", strlen("timeset")) == 0 )
    {
    this->Script("if [%s itemcget %s -open] "
                 "{%s closetree %s} else {%s opentree %s}", 
                 this->Tree->GetWidgetName(), item,
                 this->Tree->GetWidgetName(), item,
                 this->Tree->GetWidgetName(), item);
    return;
    }

  this->Script("%s selection set %s", this->Tree->GetWidgetName(),
               item);
  this->Script("%s itemcget %s -data", this->Tree->GetWidgetName(),
               item);
  const char* result = this->GetApplication()->GetMainInterp()->result;
  if (result[0] == '\0')
    {
    return;
    }

  int index[2];
  sscanf(result, "%d %d", &(index[0]), &(index[1]));

  this->SetTimeSetsFromReader();
  this->SetTimeValue(this->TimeSets->GetItem(index[0])->GetTuple1(index[1]));
  this->ModifiedCallback();
}

//-----------------------------------------------------------------------------
void vtkPVFoamSelectTimeSet::AddRootNode(const char* name, const char* text)
{
  if (!this->GetApplication())
    {
    return;
    }
  this->Script("%s insert end root %s -text {%s}", this->Tree->GetWidgetName(),
               name, text);
}

//-----------------------------------------------------------------------------
void vtkPVFoamSelectTimeSet::AddChildNode(const char* parent, const char* name, 
                                      const char* text, const char* data)
{
  if (!this->GetApplication())
    {
    return;
    }
  this->Script("%s insert end %s %s -text {%s} -data %s", 
               this->Tree->GetWidgetName(), parent, name, text, data);
}


//-----------------------------------------------------------------------------
void vtkPVFoamSelectTimeSet::SaveInBatchScript(ofstream *file)
{
  *file << "  [$pvTemp" << this->PVSource->GetVTKSourceID(0) 
        <<  " GetProperty " << this->SetCommand << "] SetElements1 "
        << this->Property->GetScalar(0) << endl;
}

//-----------------------------------------------------------------------------
void vtkPVFoamSelectTimeSet::AcceptInternal(vtkClientServerID sourceID)
{
  if (this->ModifiedFlag)
    {
    this->Script("%s selection get", this->Tree->GetWidgetName());
    this->AddTraceEntry("$kw(%s) SetTimeValueCallback {%s}", 
                        this->GetTclName(), 
                        this->GetApplication()->GetMainInterp()->result);
    }

  this->Property->SetVTKSourceID(sourceID);
  this->Property->SetScalars(1, &this->TimeValue);
  this->Property->AcceptInternal();

  this->ModifiedFlag = 0;
}

//---------------------------------------------------------------------------
void vtkPVFoamSelectTimeSet::Trace(ofstream *file)
{
  if ( ! this->InitializeTrace(file))
    {
    return;
    }

  this->Script("%s selection get", this->Tree->GetWidgetName());
  *file << "$kw(" << this->GetTclName() << ") SetTimeValueCallback {"
        << this->GetApplication()->GetMainInterp()->result << "}" << endl;
}


//-----------------------------------------------------------------------------
void vtkPVFoamSelectTimeSet::ResetInternal()
{
  if ( ! this->ModifiedFlag)
    {
    return;
    }

  // Command to update the UI.
  if (!this->Tree)
    {
    return;
    }

  this->Script("%s delete [%s nodes root]", this->Tree->GetWidgetName(),
               this->Tree->GetWidgetName());
  
  this->SetTimeSetsFromReader();

  int timeSetId=0;
  char timeSetName[32];
  char timeSetText[32];

  char timeValueName[32];
  char timeValueText[32];
  char indices[32];

  float actualTimeValue = this->Property->GetScalar(0);
  int matchFound = 0;

  this->ModifiedFlag = 0;

  if (this->TimeSets->GetNumberOfItems() == 0)
    {
    this->Script("pack forget %s", this->TreeFrame->GetWidgetName());
    this->TimeLabel->SetLabel("No timesets available.");
    return;
    }
  else
    {
    this->SetTimeValue(actualTimeValue);
    this->Script("pack %s -expand t -fill x", this->TreeFrame->GetWidgetName());
    }

  this->TimeSets->InitTraversal();
  vtkDataArray* da;
  while( (da=this->TimeSets->GetNextItem()) )
    {
    timeSetId++;
    sprintf(timeSetName,"timeset%d", timeSetId);
    sprintf(timeSetText,"Time Set %d", timeSetId); 
    this->AddRootNode(timeSetName, timeSetText);
    
    vtkIdType tuple;
    for(tuple=0; tuple<da->GetNumberOfTuples(); tuple++)
      {
      float timeValue = da->GetTuple1(tuple);
      sprintf(timeValueName, "time%d_%-12.5e", timeSetId, timeValue);
      sprintf(timeValueText, "%-12.5e", timeValue);
      ostrstream str;
      str << "{" << timeSetId-1 << " " << tuple << "}" << ends;
      sprintf(indices, "%s", str.str());
      str.rdbuf()->freeze(0);
      this->AddChildNode(timeSetName, timeValueName, timeValueText, indices);
      if (actualTimeValue == timeValue && !matchFound)
        {
        matchFound=1;
        this->Script("%s selection set %s", this->Tree->GetWidgetName(),
                     timeValueName);
        }
      }
    if (timeSetId == 1)
      {
      this->Script("%s opentree %s", this->Tree->GetWidgetName(), 
                   timeSetName);
      }
    }
  
  this->SetTimeValue(actualTimeValue);
}

//-----------------------------------------------------------------------------
void vtkPVFoamSelectTimeSet::AddAnimationScriptsToMenu(vtkKWMenu *menu, 
                                                   vtkPVAnimationInterfaceEntry *ai)
{
  char methodAndArgs[500];

  sprintf(methodAndArgs, "AnimationMenuCallback %s", ai->GetTclName()); 
  // I do not under stand why the trace name is used for the
  // menu entry, but Berk must know.
  menu->AddCommand(this->GetTraceName(), this, methodAndArgs, 0, "");
}


//-----------------------------------------------------------------------------
// What a pain.  I need this method for tracing.
// Maybe the animation should call PVwidget methods and not vtk object methods.
void vtkPVFoamSelectTimeSet::AnimationMenuCallback(vtkPVAnimationInterfaceEntry *ai)
{
  if (ai->InitializeTrace(NULL))
    {
    this->AddTraceEntry("$kw(%s) AnimationMenuCallback $kw(%s)", 
                        this->GetTclName(), ai->GetTclName());
    }
  
  // I do not under stand why the trace name is used for the
  // menu entry, but Berk must know.
  ai->SetLabelAndScript(this->GetTraceName(), NULL, this->GetTraceName());
  ai->SetCurrentProperty(this->Property);
  ai->Update();
}



//-----------------------------------------------------------------------------
vtkPVFoamSelectTimeSet* vtkPVFoamSelectTimeSet::ClonePrototype(vtkPVSource* pvSource,
                                 vtkArrayMap<vtkPVWidget*, vtkPVWidget*>* map)
{
  vtkPVWidget* clone = this->ClonePrototypeInternal(pvSource, map);
  return vtkPVFoamSelectTimeSet::SafeDownCast(clone);
}

//-----------------------------------------------------------------------------
void vtkPVFoamSelectTimeSet::CopyProperties(vtkPVWidget* clone, 
                                      vtkPVSource* pvSource,
                              vtkArrayMap<vtkPVWidget*, vtkPVWidget*>* map)
{
  this->Superclass::CopyProperties(clone, pvSource, map);
  vtkPVFoamSelectTimeSet* pvts = vtkPVFoamSelectTimeSet::SafeDownCast(clone);
  if (pvts)
    {
    pvts->SetLabel(this->FrameLabel);
    pvts->SetSetCommand(this->SetCommand);
    }
  else 
    {
    vtkErrorMacro(
      "Internal error. Could not downcast clone to PVFoamSelectTimeSet.");
    }
}

//-----------------------------------------------------------------------------
int vtkPVFoamSelectTimeSet::ReadXMLAttributes(vtkPVXMLElement* element,
                                        vtkPVXMLPackageParser* parser)
{
  if(!this->Superclass::ReadXMLAttributes(element, parser)) { return 0; }
  
  // Setup the Label.
  const char* label = element->GetAttribute("label");
  if(label)
    {
    this->SetLabel(label);
    }
  
  this->SetSetCommand(element->GetAttribute("set_command"));
  
  return 1;
}

//-----------------------------------------------------------------------------
void vtkPVFoamSelectTimeSet::SetTimeSetsFromReader()
{
  vtkPVProcessModule* pm = this->GetPVApplication()->GetProcessModule();
  this->TimeSets->RemoveAllItems();

  // Create the server-side helper if necessary.
  if(!this->ServerSideID.ID)
    {
    this->ServerSideID = pm->NewStreamObject("vtkPVFoamServerSelectTimeSet");
    pm->SendStream(vtkProcessModule::DATA_SERVER);
    }

  // Get the time sets from the reader on the server.
  // Reader -> VTKSourceID (0). We assume that there is 1 VTKSource.
  pm->GetStream() << vtkClientServerStream::Invoke
                  << this->ServerSideID << "GetTimeSets"
                  << this->PVSource->GetVTKSourceID(0)
                  << vtkClientServerStream::End;
  pm->SendStream(vtkProcessModule::DATA_SERVER_ROOT);
  vtkClientServerStream timeSets;
  if(!pm->GetLastServerResult().GetArgument(0, 0, &timeSets))
    {
    vtkErrorMacro("Error getting time sets from server.");
    return;
    }

  // There is one time set per message.
  for(int m=0; m < timeSets.GetNumberOfMessages(); ++m)
    {
    // Each argument in the message is a time set entry.
    vtkFloatArray* timeSet = vtkFloatArray::New();
    int n = timeSets.GetNumberOfArguments(m);
    timeSet->SetNumberOfTuples(n);
    for(int i=0; i < n; ++i)
      {
      float value;
      if(!timeSets.GetArgument(m, i, &value))
        {
        vtkErrorMacro("Error reading time set value.");
        timeSet->Delete();
        return;
        }
      timeSet->SetTuple1(i, value);
      }
    this->TimeSets->AddItem(timeSet);
    timeSet->Delete();
    }
  
  if (this->Property->GetNumberOfScalars() == 0 &&
      this->TimeSets->GetNumberOfItems() > 0)
    {
    vtkFloatArray *ts =
      vtkFloatArray::SafeDownCast(this->TimeSets->GetItem(0));
    this->Property->SetScalars(1, ts->GetPointer(0));
    }
}

//----------------------------------------------------------------------------
void vtkPVFoamSelectTimeSet::SaveInBatchScriptForPart(ofstream *file,
                                                  vtkClientServerID sourceID)
{
  if (sourceID.ID == 0)
    {
    vtkErrorMacro(<< this->GetClassName()
                  << " must not have SaveInBatchScript method.");
    return;
    } 

  *file << "\t" << "pvTemp" << sourceID
        << " SetTimeValue " << this->GetTimeValue()
        << endl;;
}

//-----------------------------------------------------------------------------
void vtkPVFoamSelectTimeSet::SetProperty(vtkPVWidgetProperty *prop)
{
  this->Property = vtkPVScalarListWidgetProperty::SafeDownCast(prop);
  if (this->Property)
    {
    int numScalars = 1;
    this->Property->SetVTKCommands(1, &this->SetCommand, &numScalars);
    }
}

//-----------------------------------------------------------------------------
vtkPVWidgetProperty* vtkPVFoamSelectTimeSet::GetProperty()
{
  return this->Property;
}

//-----------------------------------------------------------------------------
vtkPVWidgetProperty* vtkPVFoamSelectTimeSet::CreateAppropriateProperty()
{
  return vtkPVScalarListWidgetProperty::New();
}

//-----------------------------------------------------------------------------
void vtkPVFoamSelectTimeSet::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "TimeValue: " << this->TimeValue << endl;
  os << indent << "LabeledFrame: " << this->LabeledFrame << endl;
  os << indent << "SetCommand: "
     << (this->SetCommand ? this->SetCommand : "(none)") << endl;
}

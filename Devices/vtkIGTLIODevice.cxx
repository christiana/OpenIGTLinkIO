/*==========================================================================

  Portions (c) Copyright 2008-2009 Brigham and Women's Hospital (BWH) All Rights Reserved.

  See Doc/copyright/copyright.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

==========================================================================*/

#include "vtkIGTLIODevice.h"

// OpenIGTLink includes
#include <igtlMessageBase.h>

// VTK includes
#include <vtkObjectFactory.h>
#include <vtksys/SystemTools.hxx>
#include <vtkTimerLog.h>

// STD includes
#include <string>


////---------------------------------------------------------------------------
//vtkStandardNewMacro(vtkIGTLIODevice);

////---------------------------------------------------------------------------
//class vtkIGTLIODevicePrivate
//{
//public:
//  vtkIGTLIODevicePrivate();
//  ~vtkIGTLIODevicePrivate();

//  void SetOpenIGTLinkIFLogic(vtkSlicerOpenIGTLinkIFLogic* logic);
//  vtkSlicerOpenIGTLinkIFLogic* GetOpenIGTLinkIFLogic();

//protected:
//  vtkSlicerOpenIGTLinkIFLogic* OpenIGTLinkIFLogic;
//};

//vtkIGTLIODevicePrivate::vtkIGTLIODevicePrivate()
//{
//  this->OpenIGTLinkIFLogic = NULL;
//}

//vtkIGTLIODevicePrivate::~vtkIGTLIODevicePrivate()
//{
//}

//void vtkIGTLIODevicePrivate::SetOpenIGTLinkIFLogic(vtkSlicerOpenIGTLinkIFLogic* logic)
//{
//  this->OpenIGTLinkIFLogic = logic;
//}


//vtkSlicerOpenIGTLinkIFLogic* vtkIGTLIODevicePrivate::GetOpenIGTLinkIFLogic()
//{
//  return this->OpenIGTLinkIFLogic;
//}


//---------------------------------------------------------------------------
std::vector<vtkIGTLIODevice::QueryType> vtkIGTLIODevice::GetQueries() const
{
  return Queries;
}

int vtkIGTLIODevice::CheckQueryExpiration()
{
  double currentTime = vtkTimerLog::GetUniversalTime();
//  if (this->QueryWaitingQueue.size() > 0)
//    {
//    for (std::list< vtkWeakPointer<vtkMRMLIGTLQueryNode> >::iterator iter = this->QueryWaitingQueue.begin();
//      iter != this->QueryWaitingQueue.end(); /* increment in the loop to allow erase */ )
//      {
//      if (iter->GetPointer()==NULL)
//        {
//        // the node has been deleted, so remove it from the list
//        iter = this->QueryWaitingQueue.erase(iter);
//        continue;
//        }
  bool expired = false;

  for (unsigned i=0; i<Queries.size(); ++i)
    {
      double timeout = this->GetQueryTimeOut();
      if ((timeout>0)
          && (currentTime-Queries[i].status>timeout)
          && (Queries[i].status==QUERY_STATUS_WAITING))
        {
        Queries[i].status=QUERY_STATUS_EXPIRED;
        expired = true;
        }

    }

  if (expired)
    this->InvokeEvent(ResponseEvent);

  return 0;
}

int vtkIGTLIODevice::PruneCompletedQueries()
{
  std::vector<QueryType> pruned;

  for (int i=0; i<Queries.size(); ++i)
    if (Queries[i].status == QUERY_STATUS_WAITING)
      pruned.push_back(Queries[i]);

  if (pruned.size()!=Queries.size())
    this->Modified();

  Queries = pruned;
  return 0;
}

int vtkIGTLIODevice::CancelQuery(int index)
{
  Queries.erase(Queries.begin()+index);
  return 0;
}

vtkIGTLIODevice::vtkIGTLIODevice()
{
  PushOnConnect = false;
  MessageDirection = MESSAGE_DIRECTION_IN;
  QueryTimeOut = 0;
//  this->CheckCRC = 1;
//  this->Private = new vtkIGTLIODevicePrivate;
}

//---------------------------------------------------------------------------
vtkIGTLIODevice::~vtkIGTLIODevice()
{
//  if (this->Private)
//    {
//    delete this->Private;
//    }
}

////---------------------------------------------------------------------------
//void vtkIGTLIODevice::PrintSelf(ostream& os, vtkIndent indent)
//{
//  this->vtkObject::PrintSelf(os, indent);
//}


////---------------------------------------------------------------------------
//void vtkIGTLIODevice::SetOpenIGTLinkIFLogic(vtkSlicerOpenIGTLinkIFLogic* logic)
//{
//  if (this->Private)
//    {
//    this->Private->SetOpenIGTLinkIFLogic(logic);
//    }
//}


////---------------------------------------------------------------------------
//vtkSlicerOpenIGTLinkIFLogic* vtkIGTLIODevice::GetOpenIGTLinkIFLogic()
//{
//  if (this->Private)
//    {
//    return this->Private->GetOpenIGTLinkIFLogic();
//    }
//  else
//    {
//    return NULL;
//    }
//}

////---------------------------------------------------------------------------
//int vtkIGTLIODevice::IGTLToMRML(igtl::MessageBase::Pointer buffer, vtkMRMLNode* node)
//{
//  if(buffer && node)
//    {
//      igtlUint32 second;
//      igtlUint32 nanosecond;
      
//      buffer->GetTimeStamp(&second, &nanosecond);
      
//      std::stringstream ss;
//      ss << second << nanosecond;
      
//      node->SetAttribute("Timestamp",ss.str().c_str());
//    }
//  return 0;
//}


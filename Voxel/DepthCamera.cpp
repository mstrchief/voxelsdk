/*
 * TI Voxel Lib component.
 *
 * Copyright (c) 2014 Texas Instruments Inc.
 */

#include "DepthCamera.h"
#include "Logger.h"

namespace Voxel
{
  
bool DepthCamera::_addParameters(const Vector<ParameterPtr> &params)
{
  _parameters.reserve(_parameters.size() + params.size());
  
  for(const ParameterPtr &p: params)
  {
    if(_parameters.find(p->name()) == _parameters.end())
    {
      _parameters[p->name()] = p;
    }
    else
    {
      logger(ERROR) << "DepthCamera: Found an existing parameter in the list of parameters, with name " << p->name() << ". Not overwriting it." << endl;
      return false;
    }
  }
  return true;
}

bool DepthCamera::clearCallback()
{
  _callback = 0;
}

bool DepthCamera::registerCallback(FrameCallBackType type, CallbackType f)
{
  _callback = f;
  _callBackType = type;
}


void DepthCamera::_captureLoop()
{
  while(_running)
  {
    if(_callBackType == CALLBACK_RAW_FRAME_UNPROCESSED)
    {
      RawFramePtr &f = _rawFrameBuffers.get();
      if(_captureRawUnprocessedFrame(f))
        _callback(*this, (Frame &)(*f), _callBackType);
    }
    else
    {
      RawFramePtr f1;
      if(!_captureRawUnprocessedFrame(f1))
        continue;
      
      RawFramePtr &f = _rawFrameBuffers.get();
      
      if(!_processRawFrame(f1, f))
      {
        _rawFrameBuffers.release(f);
        continue;
      }

      if(_callBackType == CALLBACK_RAW_FRAME_PROCESSED)
      {
        _callback(*this, (Frame &)(*f), _callBackType);
        _rawFrameBuffers.release(f);
        continue;
      }
      
      
      DepthFramePtr &d = _depthFrameBuffers.get();
      
      if(!_convertToDepthFrame(f, d))
      {
        _rawFrameBuffers.release(f);
        _depthFrameBuffers.release(d);
        continue;
      }
      
      _rawFrameBuffers.release(f);
      
      if(_callBackType == CALLBACK_DEPTH_FRAME)
      {
        _callback(*this, (Frame &)(*d), _callBackType);
        _depthFrameBuffers.release(d);
        continue;
      }
      
      PointCloudFramePtr &p = _pointCloudBuffers.get();
      
      if(!_convertToPointCloudFrame(d, p))
      {
        _depthFrameBuffers.release(d);
        _pointCloudBuffers.release(p);
        continue;
      }
      
      _depthFrameBuffers.release(d);
      
      _callback(*this, (Frame &)(*p), _callBackType); // point cloud type callback
      
      _pointCloudBuffers.release(p);
    }
  }
  
  if(!_running)
  {
    _stop();
  }
}

bool DepthCamera::_convertToPointCloudFrame(DepthFramePtr &depthFrame, PointCloudFramePtr &pointCloudFrame)
{
  return false;
}


void DepthCamera::_captureThreadWrapper()
{
  _captureLoop();
}

bool DepthCamera::start()
{
  if(!_callback)
  {
    logger(ERROR) << "DepthCamera: Please register a callback to " << _id << " before starting capture" << std::endl;
    return false;
  }
  
  if(!_start())
    return false;
  
  _running = true;
  
  //_captureThreadWrapper();
  _captureThread = ThreadPtr(new Thread(&DepthCamera::_captureThreadWrapper, this));
  
  return true;
}

bool DepthCamera::stop()
{
  _running = false;
  return true;
}

void DepthCamera::wait()
{
  if(isRunning())
    _captureThread->join();
}

DepthCamera::~DepthCamera()
{
  _rawFrameBuffers.clear();
  _depthFrameBuffers.clear();
  _pointCloudBuffers.clear();
  
  _parameters.clear();
}

  
}
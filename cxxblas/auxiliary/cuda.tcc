#ifndef CXXBLAS_AUXILIARY_CUDA_TCC
#define CXXBLAS_AUXILIARY_CUDA_TCC 1

#if defined(WITH_CUBLAS) || defined(WITH_CUSOLVER)

// XXX
#include <utility>
#include <sstream>

namespace flens {

void
CudaEnv::init(){
    if(NCalls==0) {
#ifdef WITH_CUBLAS
        // create BLAS handle
        cublasStatus_t cublas_status = cublasCreate(&blas_handle);
        checkStatus(cublas_status);
#endif // WITH_CUBLAS
#ifdef WITH_CUSOLVER
        // create SOLVER handle
        cusolverStatus_t cusolver_status = cusolverDnCreate(&solver_handle);
        checkStatus(cusolver_status);
#endif // WITH_CUSOLVER
        // Create stream with index 0
        streamID   = 0;
        streams.insert(std::make_pair(streamID, cudaStream_t()));
        cudaError_t cuda_status = cudaStreamCreate(&streams.at(0));
        checkStatus(cuda_status);
    }
    NCalls++;
}


void
CudaEnv::release(){
    ASSERT(NCalls>0);
    if (NCalls==1) {

        // destroy events
        cudaError_t cuda_status = cudaSuccess;
        for (std::map<int, cudaEvent_t >::iterator it=events.begin(); it!=events.end(); ++it) {
            cuda_status = cudaEventDestroy(it->second);
            checkStatus(cuda_status);
        }
        events.clear();

        // destroy stream
        for (std::map<int, cudaStream_t>::iterator it=streams.begin(); it!=streams.end(); ++it) {
            cuda_status = cudaStreamDestroy(it->second);
            checkStatus(cuda_status);
        }
        streams.clear();

#ifdef WITH_CUBLAS
        // destroy BLAS handle
        cublasStatus_t cublas_status = cublasDestroy(blas_handle);
        checkStatus(cublas_status);
#endif // WITH_CUBLAS
#ifdef WITH_CUSOLVER
        // destroy SOLVER handle
        cusolverStatus_t cusolver_status = cusolverDnDestroy(solver_handle);
        checkStatus(cublas_status);
#endif // WITH_CUSOLVER
    }
    NCalls--;
}


void
CudaEnv::destroyStream(int _streamID)
{
    if(NCalls==0) {
        std::cerr << "Error: Cuda not initialized!" << std::endl;
        ASSERT(0);
    }

    ASSERT(_streamID!=streamID);
    cudaError_t cuda_status = cudaStreamDestroy(streams.at(_streamID));
    checkStatus(cuda_status);
    streams.erase(_streamID);
}

#ifdef WITH_CUBLAS
cublasHandle_t &
CudaEnv::blasHandle()
{
    if(NCalls==0) {
        std::cerr << "Error: Cuda not initialized!" << std::endl;
        ASSERT(0);
    }

    return blas_handle;
}
#endif // WITH_CUBLAS

#ifdef WITH_CUSOLVER
cusolverDnHandle_t &
CudaEnv::solverHandle()
{
    if(NCalls==0) {
        std::cerr << "Error: Cuda not initialized!" << std::endl;
        ASSERT(0);
    }

    return solver_handle;
}
#endif // WITH_CUSOLVER

cudaStream_t &
CudaEnv::getStream()
{
    if(NCalls==0) {
        std::cerr << "Error: Cuda not initialized!" << std::endl;
        ASSERT(0);
    }

    return streams.at(streamID);
}

int
CudaEnv::getStreamID()
{
    if(NCalls==0) {
        std::cerr << "Error: Cuda not initialized!" << std::endl;
        ASSERT(0);
    }

    return streamID;
}


void
CudaEnv::setStream(int _streamID)
{
    if(NCalls==0) {
        std::cerr << "Error: Cuda not initialized!" << std::endl;
        ASSERT(0);
    }

    streamID = _streamID;
    // Create new stream, object not found
    if (streams.find(streamID) == streams.end()) {
        streams.insert(std::make_pair(streamID, cudaStream_t()));
        cudaError_t cuda_status = cudaStreamCreate(&streams.at(streamID));
        checkStatus(cuda_status);
    }

#ifdef WITH_CUBLAS
    // Set stream
    cublasStatus_t cublas_status = cublasSetStream(blas_handle, streams.at(streamID));
    checkStatus(cublas_status);
#endif
}

void
CudaEnv::syncStream(int _streamID)
{
    if(NCalls==0) {
        std::cerr << "Error: Cuda not initialized!" << std::endl;
        ASSERT(0);
    }

    cudaError_t cuda_status = cudaStreamSynchronize(streams.at(_streamID));
    checkStatus(cuda_status);
}

void
CudaEnv::enableSyncCopy()
{
    syncCopyEnabled = true;
}

void
CudaEnv::disableSyncCopy()
{
    syncCopyEnabled = false;
}

bool
CudaEnv::isSyncCopyEnabled()
{
    return syncCopyEnabled;
}


void
CudaEnv::eventRecord(int _eventID)
{
    ///
    /// Creates event on current stream
    ///
    cudaError_t cuda_status;
    // Insert new event
    if (events.find(_eventID) == events.end()) {
        events.insert(std::make_pair(_eventID, cudaEvent_t ()));
        cuda_status = cudaEventCreate(&events.at(_eventID));
        checkStatus(cuda_status);

    }

    // Create Event
    cuda_status = cudaEventRecord(events.at(_eventID), streams.at(streamID));
    checkStatus(cuda_status);
}

void
CudaEnv::eventSynchronize(int _eventID)
{
     ///
     /// cudaEventSynchronize: Host waits until -eventID is completeted
     ///
     ///
     cudaError_t cuda_status = cudaEventSynchronize(events.at(_eventID));
     checkStatus(cuda_status);
}

std::string
CudaEnv::getInfo()
{
    std::stringstream sstream;
    // Number of CUDA devices
    int devCount;
    cudaGetDeviceCount(&devCount);

    // Iterate through devices
    for (int i = 0; i < devCount; ++i)
    {
        // Get device properties
        cudaDeviceProp devProp;
        cudaGetDeviceProperties(&devProp, i);

        sstream << "CUDA Device " << devProp.name << std::endl;
        sstream << "==============================================" << std::endl;
        sstream << "Major revision number:         " << std::setw(15) << devProp.major << std::endl;
        sstream << "Minor revision number:         " << std::setw(15) << devProp.minor << std::endl;
        sstream << "Number of multiprocessors:     " << std::setw(15) << devProp.multiProcessorCount << std::endl;
        sstream << "Clock rate:                    " << std::setw(11) << devProp.clockRate / (1024.f*1024.f) << " GHz" << std::endl;
        sstream << "Total global memory:           " << std::setw(9) << devProp.totalGlobalMem / (1024*1024) << " MByte" << std::endl;
        sstream << "Total constant memory:         " << std::setw(9) << devProp.totalConstMem  / (1024) << " KByte"<< std::endl;
        sstream << "Maximum memory:                " << std::setw(9) << devProp.memPitch  / (1024*1024) << " MByte"<< std::endl;
        sstream << "Total shared memory per block: " << std::setw(9) << devProp.sharedMemPerBlock / (1024) << " KByte" << std::endl;
        sstream << "Total registers per block:     " << std::setw(15) << devProp.regsPerBlock << std::endl;
        sstream << "Warp size:                     " << std::setw(15) << devProp.warpSize << std::endl;
        sstream << "Maximum threads per block:     " << std::setw(15) << devProp.maxThreadsPerBlock << std::endl;
        for (int j = 0; j < 3; ++j) {
            sstream << "Maximum dimension of block " << j << ":  " << std::setw(15) << devProp.maxThreadsDim[j] << std::endl;
        }
        for (int j = 0; j < 3; ++j) {
            sstream << "Maximum dimension of grid " << j << ":   "  << std::setw(15) << devProp.maxGridSize[j] << std::endl;
        }
        sstream << "Texture alignment:             " << std::setw(15) << devProp.textureAlignment << std::endl;
        sstream << "Concurrent copy and execution: " << std::setw(15) << std::boolalpha << (devProp.deviceOverlap>0) << std::endl;
        sstream << "Concurrent kernel execution:   " << std::setw(15) << std::boolalpha << (devProp.concurrentKernels>0) << std::endl;
        sstream << "Kernel execution timeout:      " << std::setw(15) << std::boolalpha << (devProp.kernelExecTimeoutEnabled>0) << std::endl;
        sstream << "ECC support enabled:           " << std::setw(15) << std::boolalpha << (devProp.ECCEnabled>0) << std::endl;
    }
    return sstream.str();
}

#ifdef WITH_CUBLAS
void
checkStatus(cublasStatus_t status)
{
    if (status==CUBLAS_STATUS_SUCCESS) {
        return;
    }

    if (status==CUBLAS_STATUS_NOT_INITIALIZED) {
        std::cerr << "CUBLAS: Library was not initialized!" << std::endl;
    } else if  (status==CUBLAS_STATUS_INVALID_VALUE) {
        std::cerr << "CUBLAS: Parameter had illegal value!" << std::endl;
    } else if  (status==CUBLAS_STATUS_MAPPING_ERROR) {
        std::cerr << "CUBLAS: Error accessing GPU memory!" << std::endl;
    } else if  (status==CUBLAS_STATUS_ALLOC_FAILED) {
        std::cerr << "CUBLAS: allocation failed!" << std::endl;
    } else if  (status==CUBLAS_STATUS_ARCH_MISMATCH) {
        std::cerr << "CUBLAS: Device does not support double precision!" << std::endl;
    } else if  (status==CUBLAS_STATUS_EXECUTION_FAILED) {
        std::cerr << "CUBLAS: Failed to launch function of the GPU" << std::endl;
    } else if  (status==CUBLAS_STATUS_INTERNAL_ERROR) {
        std::cerr << "CUBLAS: An internal operation failed" << std::endl;
    } else {
        std::cerr << "CUBLAS: Unkown error" << std::endl;
    }

    ASSERT(status==CUBLAS_STATUS_SUCCESS); // false
}
#endif // WITH_CUBLAS

#ifdef WITH_CUSOLVER
void
checkStatus(cusolverStatus_t status)
{
    if (status==CUSOLVER_STATUS_SUCCESS) {
        return;
    }

    if (status==CUSOLVER_STATUS_NOT_INITIALIZED) {
        std::cerr << "CUSOLVER: Library was not initialized!" << std::endl;
    } else if  (status==CUSOLVER_STATUS_INVALID_VALUE) {
        std::cerr << "CUSOLVER: Parameter had illegal value!" << std::endl;
    } else if  (status==CUSOLVER_STATUS_ALLOC_FAILED) {
        std::cerr << "CUSOLVER: allocation failed!" << std::endl;
    } else if  (status==CUSOLVER_STATUS_ARCH_MISMATCH) {
        std::cerr << "CUSOLVER: Device does not support double precision!" << std::endl;
    } else if  (status==CUSOLVER_STATUS_EXECUTION_FAILED) {
        std::cerr << "CUSOLVER: Failed to launch function of the GPU" << std::endl;
    } else if  (status==CUSOLVER_STATUS_INTERNAL_ERROR) {
        std::cerr << "CUSOLVER: An internal operation failed" << std::endl;
    } else if  (status==CUSOLVER_STATUS_MATRIX_TYPE_NOT_SUPPORTED) {
        std::cerr << "CUSOLVER: Invalid matrix descriptor" << std::endl;
    } else {
        std::cerr << "CUSOLVER: Unkown error" << std::endl;
    }

    ASSERT(status==CUSOLVER_STATUS_SUCCESS); // false
}
#endif // WITH_CUSOLVER

void
checkStatus(cudaError_t status)
{
    if(status==cudaSuccess)
        return;
    std::cerr << cudaGetErrorString(status) << std::endl;
}

void
setStream(int streamID)
{
    CudaEnv::setStream(streamID);
}

int
getStreamID()
{
    return CudaEnv::getStreamID();
}

void
destroyStream(int streamID)
{
    CudaEnv::destroyStream(streamID);
}

template <typename... Args>
void
destroyStream(int streamID, Args... args)
{
    destroyStream(streamID);
    destroyStream(args...);
}

void
syncStream()
{
    syncStream(getStreamID());
}

void
syncStream(int streamID)
{
    CudaEnv::syncStream(streamID);
}

template <typename... Args>
void
syncStream(int streamID, Args... args)
{
    syncStream(streamID);
    syncStream(args...);
}


} // end namespace flens

#endif // WITH_CUDA

#endif

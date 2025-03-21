#include "FramedSource.hh"

// 使用相对路径包含方式避免绝对路径依赖
#include "opencv2/opencv.hpp"

#ifdef _WIN32
// Windows 实现
int gettimeofday(struct timeval* tp, void* tzp) {
    (void)tzp;
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    ULARGE_INTEGER uli;
    uli.LowPart = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;
    ULONGLONG usecs = (uli.QuadPart - 116444736000000000ULL) / 10;
    tp->tv_sec = (long)(usecs / 1000000);
    tp->tv_usec = (long)(usecs % 1000000);
    return 0;
}
#else
// 非 Windows 平台直接使用 gettimeofday
#include <sys/time.h>
#endif

class CameraFramedSource : public FramedSource {
public:
    static CameraFramedSource* createNew(UsageEnvironment& env) {
        return new CameraFramedSource(env);
    }

protected:
    CameraFramedSource(UsageEnvironment& env)
        : FramedSource(env), cap(0) {  // 尝试打开默认摄像头设备
        printf("[Camera] Trying to open video capture device...\n");
        
        // 增加多平台设备号处理
        #ifdef _WIN32
        int device_id = 0; // Windows通常使用0作为默认摄像头
        #else 
        int device_id = -1; // Linux可能需要使用V4L2路径
        #endif
        
        if (!cap.open(device_id)) {
            env << "ERROR: Failed to open video capture device!\n";
            env << "Possible reasons:\n";
            env << "1. Camera not connected\n";
            env << "2. No permission to access camera\n";
            env << "3. Invalid device ID (" << device_id << ")\n";
            handleClosure();
            return;
        }
        
        // 检查摄像头参数是否有效
        int width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
        int height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
        printf("[Camera] Opened successfully. Resolution: %dx%d\n", width, height);
    }

    virtual ~CameraFramedSource() {
        cap.release();
    }

private:
#if 0
    virtual void doGetNextFrame() {
        printf("CameraFramedSource::doGetNextFrame\n");
        // 读取帧数据
        cv::Mat frame;
        if (cap.read(frame)) {
            // 将帧数据复制到 fTo
            fFrameSize = frame.total() * frame.elemSize();
            if (fFrameSize > fMaxSize) {
                fFrameSize = fMaxSize;
            }
            memcpy(fTo, frame.data, fFrameSize);

            // 设置帧的显示时间
            gettimeofday(&fPresentationTime, NULL);
        } else {
            // 处理错误
            handleClosure();
        }

        // 通知读取完成
        FramedSource::afterGetting(static_cast<FramedSource*>(this));
    }
#else
     virtual void doGetNextFrame() {
        cv::Mat frame;
        if (cap.read(frame)) {
            // 转换为 YUV 格式（或直接使用 BGR）
            cv::cvtColor(frame, frame, cv::COLOR_BGR2YUV_I420);

            // 将帧数据拷贝到LIVE555的 buffer
            memmove(fTo, frame.data, frame.total());
            fFrameSize = frame.total();
            // fPresentationTime = getCurrentTime();
            // 设置帧的显示时间
            gettimeofday(&fPresentationTime, NULL);

            // 通知下游有新数据
            FramedSource::afterGetting(this);
        } else {
            envir() << "Failed to read frame from webcam.\n";
        }
    }
#endif
    cv::VideoCapture cap;  // 需要确保OpenCV库正确链接
};

#include <nan.h>
#include <string>
#include <math.h>

namespace __transform__ {

using v8::Local;
using v8::Object;
using v8::String;

const double pi = 3.14159265358979324;
const double a = 6378245.0;
const double ee = 0.00669342162296594323;
const  double x_pi = 3.14159265358979324 * 3000.0 / 180.0;
  


bool outOfChina(double lon,double lat){
    return !(lon>72.004&&lon<137.8347&&lat>0.8292&&lat<55.8272);
}

NAN_METHOD(WGS84ToGoogle)
{
    if(!info.Length()) return;

    auto input = info[0]->ToObject();
    auto output = Nan::New<Object>();

    auto x_prop = Nan::New("x").ToLocalChecked();
    auto y_prop = Nan::New("y").ToLocalChecked();
    auto lon_prop = Nan::New("lon").ToLocalChecked();
    auto lat_prop = Nan::New("lat").ToLocalChecked();

    double x = Nan::Get(input, x_prop).ToLocalChecked()->NumberValue();
    double y = Nan::Get(input, y_prop).ToLocalChecked()->NumberValue();
    
    if(outOfChina(x,y)){
        Nan::Set(output, lon_prop, Nan::New(x));
        Nan::Set(output, lat_prop, Nan::New(y));
    }else{
        double dx=x;
        double dy=y;
        x-=105.0;
        y-=35.0;
       
        double xy=x*y;
        double absX=sqrt(abs(x));
        double xPi=x*pi;
        double yPi=y*pi;
        double d = 20.0 * sin(6.0 * xPi) + 20.0 * sin(2.0 * xPi);
        double lat = d;
        double lng = d;
        lat += 20.0 * sin(yPi) + 40.0 * sin(yPi / 3.0);
        lng += 20.0 * sin(xPi) + 40.0 * sin(xPi / 3.0);

        lat += 160.0 * sin(yPi / 12.0) + 320 * sin(yPi / 30.0);
        lng += 150.0 * sin(xPi / 12.0) + 300.0 * sin(xPi / 30.0);

        lat *= 2.0 / 3.0;
        lng *= 2.0 / 3.0;

        lat += -100.0 + 2.0 * x + 3.0 * y + 0.2 * y * y + 0.1 * xy + 0.2 * absX;
        lng += 300.0 + x + 2.0 * y + 0.1 * x * x + 0.1 * xy + 0.1 * absX;
        

        double radLat = dy / 180.0 * pi;
        double magic = sin(radLat);
        magic = 1 - ee * magic * magic;
        double sqrtMagic = sqrt(magic);
        lat = (lat * 180.0) / ((a * (1 - ee)) / (magic * sqrtMagic) * pi);
        lng = (lng * 180.0) / (a / sqrtMagic * cos(radLat) * pi);
        
    Nan::Set(output, lon_prop, Nan::New(dx+lng));
    Nan::Set(output, lat_prop, Nan::New(dy+lat));
    }
    info.GetReturnValue().Set(output);
}

NAN_MODULE_INIT(Init)
{
    Nan::Export(target, "WGS84ToGoogle", WGS84ToGoogle);
}

NODE_MODULE(reverse, Init)

}

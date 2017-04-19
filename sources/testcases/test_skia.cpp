#include "gtest/gtest.h"
#include <Windows.h>

#include "SkCanvas.h"
#include "SkPath.h"
#include "SkData.h"
#include "SkImage.h"
#include "SkStream.h"
#include "SkSurface.h"

#include "SkDocument.h"
#include "SkPictureRecorder.h"
#include "SkPicture.h"

//#include "Test.h"

// stub out openGl dependency, which isn't needed in this case.
extern "C"
{
	PROC WINAPI __imp_wglGetProcAddress(LPCSTR)
	{
		abort();
		return nullptr;
	}

	HGLRC WINAPI  __imp_wglGetCurrentContext()
	{
		abort();
		return nullptr;
	}
}
//void raster(int width, int height,
//	void(*draw)(SkCanvas*),
//	const char* path) {
//	sk_sp<SkSurface> rasterSurface(
//		SkSurface::MakeRasterN32Premul(width, height));
//	SkCanvas* rasterCanvas = rasterSurface->getCanvas();
//	draw(rasterCanvas);
//	sk_sp<SkImage> img(s->newImageSnapshot());
//	if (!img) { return; }
//	sk_sp<SkData> png(img->encode());
//	if (!png) { return; }
//	SkFILEWStream out(path);
//	(void)out.write(png->data(), png->size());
//}

void skpdf(int width, int height,
	void(*draw)(SkCanvas*),
	const char* path) {
	SkFILEWStream pdfStream(path);
	sk_sp<SkDocument> pdfDoc(SkDocument::MakePDF(&pdfStream));
	SkCanvas* pdfCanvas = pdfDoc->beginPage(SkIntToScalar(width),
		SkIntToScalar(height));
	draw(pdfCanvas);
	pdfDoc->close();
}

void picture(int width, int height,
	void(*draw)(SkCanvas*),
	const char* path) {
	SkPictureRecorder recorder;
	SkCanvas* recordingCanvas = recorder.beginRecording(SkIntToScalar(width),
		SkIntToScalar(height));
	draw(recordingCanvas);
	sk_sp<SkPicture> picture(recorder.finishRecordingAsPicture());
	SkFILEWStream skpStream(path);
	// Open SKP files with `SampleApp --picture SKP_FILE`
	picture->serialize(&skpStream);
}

void example(SkCanvas* canvas) {
	const SkScalar scale = 256.0f;
	const SkScalar R = 0.45f * scale;
	const SkScalar TAU = 6.2831853f;
	SkPath path;
	for (int i = 0; i < 5; ++i) {
		SkScalar theta = 2 * i * TAU / 5;
		if (i == 0) {
			path.moveTo(R * cos(theta), R * sin(theta));
		}
		else {
			path.lineTo(R * cos(theta), R * sin(theta));
		}
	}
	path.close();
	SkPaint p;
	p.setAntiAlias(true);
	canvas->clear(SK_ColorWHITE);
	canvas->translate(0.5f * scale, 0.5f * scale);
	canvas->drawPath(path, p);
}

TEST(skia, hello_world)
{
	//raster(256, 256, example, "out_raster.png");
	//ganesh(256, 256, example, "out_ganesh.png");
	skpdf(256, 256, example, "out_skpdf.pdf");
	picture(256, 256, example, "out_picture.skp");
}
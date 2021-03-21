#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdint.h>
#include <future>

template<typename T>
class Texture;

template<typename PixelT, typename ArgT>
using PixelProcessFunction = void(Texture<PixelT> * Tex, ArgT Arg, size_t X, size_t Y);

template<typename PixelT, typename ArgT>
void PixelProcessDispatch(PixelProcessFunction<PixelT, ArgT> Function, Texture<PixelT>* Tex, ArgT Arg, size_t Y_Begin, size_t Y_End) {
	for (size_t Y = Y_Begin; Y < Y_End; Y++) {
		for (size_t X = 0; X < Tex->GetWidth(); X++) {
			Function(Tex, Arg, X, Y);
		}
	}
}

template<typename T>
class Texture {
public:
	Texture(void) : Data(nullptr), Width(0), Height(0) {}

	using Pixel = T;
	const static size_t PixelSize = sizeof(Pixel);

	void Create(size_t W, size_t H) {
		Width = W;
		Height = H;
		Data = new Pixel[W * H];
	}

	void Resize(size_t W, size_t H) {
		Texture Old = *this;
		Create(W, H);
		Copy(&Old, this);
		Old.Free();
	}

	// Copies T0 to T1 starting at (0, 0)
	void Copy(Texture* T0, Texture* T1) {
		for (size_t Y = 0; Y < T0->Width; Y++) {
			for (size_t X = 0; X < T0->Width; X++) {
				T1->Write(X, Y, T0->Sample(X, Y));
			}
		}
	}

	void Free(void) {
		delete[] Data;
	}

	void Write(size_t X, size_t Y, const Pixel& Value) {
		Data[GetTextureIndex(X, Y)] = Value;
	}

	Pixel Sample(size_t X, size_t Y) {
		return Data[GetTextureIndex(X, Y)];
	}

	size_t GetTextureIndex(size_t X, size_t Y) {
		size_t Index = Y * Width + X;
		return Index;
	}

	Pixel* GetRawData(void) {
		return Data;
	}

	void Clear(const Pixel& Value) {
		for (size_t Y = 0; Y < Height; Y++) {
			for (size_t X = 0; X < Width; X++) {
				Write(X, Y, Value);
			}
		}
	}

	size_t GetWidth(void) {
		return Width;
	}

	size_t GetHeight(void) {
		return Height;
	}

	void SetSize(size_t W, size_t H) {
		if (Width != W || Height != H) {
			if (Data) {
				Resize(W, H);
			} else {
				Create(W, H);
			}
		}
	}

	template<typename ArgT>
	void ForEachPixel(PixelProcessFunction<Pixel, ArgT> Function, ArgT Arg, size_t NumThreads = 1) {
		
		size_t ThreadRowSize = Height / NumThreads;
		// I should replace this with a thread pool
		std::vector<std::future<void>> WorkerThreads;
		WorkerThreads.reserve(NumThreads);

		for (size_t Y = 0; Y < GetHeight(); Y+= ThreadRowSize) {
			WorkerThreads.push_back(std::async(std::launch::async, PixelProcessDispatch<Pixel, ArgT>, Function, this, Arg, Y, Y + ThreadRowSize));
		}
	}
private:
	Pixel* Data;
	size_t Width;
	size_t Height;
};

#endif
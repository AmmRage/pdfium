// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <list>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "../public/fpdf_dataavail.h"
#include "../public/fpdf_ext.h"
#include "../public/fpdf_formfill.h"
#include "../public/fpdf_text.h"
#include "../public/fpdfview.h"

//#include <codecvt>
#include <chrono>
#include <Windows.h>

#include "ThreadPool.h"

#ifdef _WIN32
#define snprintf _snprintf
#define PATH_SEPARATOR '\\'
#else
#define PATH_SEPARATOR '/'
#endif
using namespace std;
vector<wchar_t*> get_all_files_names_within_folder(const char* search_path)
{
	vector<wchar_t*> names;
	//wchar_t search_path = folder + "/*.*";
	WIN32_FIND_DATA fd;
	wchar_t filename[1024] = { 0 };
	MultiByteToWideChar(0, 0, search_path, static_cast<int>(strlen(search_path)), filename, static_cast<int>(strlen(search_path)));
	HANDLE hFind = ::FindFirstFile((filename), &fd);
	//HANDLE hFind = ::FindFirstFileW(L"C:\\TEST\\TestFolder_CPF\\Release\\", &fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				wprintf_s(L"%s", fd.cAlternateFileName);
				names.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	else
	{
		auto errCode = GetLastError();
	}
	return names;
}

//std::string UnicodeToChar(const std::wstring &wstr)
void UnicodeToChar(unsigned short * wstr, int length, char * output)
{	
	WideCharToMultiByte(CP_ACP, 0, reinterpret_cast<wchar_t *>(wstr), length, output, length, NULL, NULL);
	for (int i = 0; i < 50; i++)
	{
		//if (output[i] != 46 && output[i] != 0 && (output[i] < 48 || output[i]> 57))
			//output[i] = 5;
		if (output[i] == ',' || output[i] == ' ' || output[i] == '$')
		{
			if (output[0] == 0)
				break;
			for (int j = i; j < 49; j++)
			{
				if (output[j] == 0)
					break;				
				output[j] = output[j+1];
			}
		}
	}
}


struct InputOutput
{
	int PageCount;
	char filename[256];
	char capture[5][50];

	byte IfCaptured;
};

void ReportProgress(int * count, int totalCount)
{
	std::thread t1([count, totalCount]{
	printf("Total count: %d\n", totalCount);
	int currentCount = 0;
	int currentPercentage = 0;
	int lastPercentage = 0;
	while (true)
	{
		currentCount = *count;
		if (currentCount >= totalCount)
			break;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		currentPercentage = static_cast<int>((float)currentCount / (float)totalCount * 100);
		if (currentPercentage > lastPercentage)
		{
			printf("\r%d", currentPercentage);
			lastPercentage = currentPercentage;
		}
	}});
	t1.detach();
}

extern "C"
{
	DLLEXPORT struct struct_Bound
	{
	public:
		double Left;
		double Top;
		double Right;
		double Bottom;
	};

	static int BoundCount;
	static std::vector<struct_Bound> CaptureBounds;

	DLLEXPORT void TestConvert(byte* textBuf, char * output)
	{
		auto shortPointer = static_cast<unsigned short*>(static_cast<void*>(textBuf));
		WideCharToMultiByte(CP_ACP, 0, reinterpret_cast<wchar_t *>(shortPointer), 100, output, 100, NULL, NULL);
	}

	DLLEXPORT void SetCaptureBounds(struct_Bound* arr, int length)
	{
		BoundCount = length;
		for (int i = 0; i < length; i++)
		{
			CaptureBounds.push_back(arr[i]);
			//arr[i].Top = 3;
			//printf_s("%f\n", arr[i].Left);
		}
	}

	DLLEXPORT struct ProcessInfo
	{
	public:
		char * filename;
		byte textBuf[40002];
		int bufLength;
		//struct_Bound* bounds;
		//int boundsCount;
		int textlengths[5];
	};

	DLLEXPORT char* GetFileContents(const char* filename, size_t* retlen) {
		FILE* file = fopen(filename, "rb");
		if (!file) {
			fprintf(stderr, "Failed to open: %s\n", filename);
			return NULL;
		}
		(void)fseek(file, 0, SEEK_END);
		size_t file_length = ftell(file);
		if (!file_length) {
			return NULL;
		}
		(void)fseek(file, 0, SEEK_SET);
		char* buffer = (char*)malloc(file_length);
		if (!buffer) {
			return NULL;
		}
		size_t bytes_read = fread(buffer, 1, file_length, file);
		(void)fclose(file);
		if (bytes_read != file_length) {
			fprintf(stderr, "Failed to read: %s\n", filename);
			free(buffer);
			return NULL;
		}
		*retlen = bytes_read;
		return buffer;
	}

	DLLEXPORT void GetOutputString(InputOutput* para, int length)
	{
		int i = 0;
		static unsigned short buf[5][256];
		ReportProgress(&i, length);
		for (; i < length; i++)
		{
			memset(buf, 0, 2560);
			//auto processInfo = (para + i);
			//printf("%s\n", processInfo->filename);
			size_t file_length = 0;
			char* file_contents = GetFileContents(para[i].filename, &file_length);
			if (!file_contents)
			{
				para[i].IfCaptured = false;
				continue;
			}
			para[i].IfCaptured = true;
			FPDF_DOCUMENT doc = FPDF_LoadMemDocument(file_contents, static_cast<int>(file_length), NULL);
			FPDF_PAGE page = FPDF_LoadPage(doc, 0);
			para[i].PageCount = FPDF_GetPageCount(doc);
			int count = 0;
			if (page)
			{
				FPDF_TEXTPAGE text_page = FPDFText_LoadPage(page);
				for (int j = 0; j < 5; j++)
				{
					count = FPDFText_GetBoundedText(text_page,
						CaptureBounds[j].Left, CaptureBounds[j].Top, CaptureBounds[j].Right, CaptureBounds[j].Bottom,
						buf[j],
						256);
					UnicodeToChar(buf[j], 25, para[i].capture[j]);
					//printf("%s", para[i].capture[j]);
				}
				FPDFText_ClosePage(text_page);
			}
			FPDF_ClosePage(page);
			FPDF_CloseDocument(doc);
			free(file_contents);
		}
	}

	DLLEXPORT void GetPageCount(InputOutput* para, int length)
	{
		int i = 0;
		static unsigned short buf[5][256];
		ReportProgress(&i, length);
		for (; i < length; i++)
		{
			para[i].IfCaptured = false;
			memset(buf, 0, 2560);
			auto processInfo = (para + i);
			//printf("%s\n", processInfo->filename);
			size_t file_length = 0;
			char* file_contents = GetFileContents(processInfo->filename, &file_length);
			if (!file_contents)
				continue;
			FPDF_DOCUMENT doc = FPDF_LoadMemDocument(file_contents, static_cast<int>(file_length), NULL);
			para[i].PageCount = FPDF_GetPageCount(doc);
			FPDF_CloseDocument(doc);
			free(file_contents);
		}
	}

	DLLEXPORT void MultiThreadPoolProcess(ProcessInfo * processInfos, int length)
	{
		int i = 0;
		for (; i < length; i++)
		{
			ProcessInfo* processInfo = processInfos + sizeof(ProcessInfo);
			size_t file_length = 0;
			char* file_contents = GetFileContents(processInfo->filename, &file_length);
			if (!file_contents)
				return ;
			FPDF_DOCUMENT doc = FPDF_LoadMemDocument(file_contents, static_cast<int>(file_length), NULL);
			FPDF_PAGE page = FPDF_LoadPage(doc, 0);
			int countSum = 0;
			int count = 0;
			if (page)
			{
				auto shortPointer = static_cast<unsigned short*>(static_cast<void*>(processInfo->textBuf));						
				FPDF_TEXTPAGE text_page = FPDFText_LoadPage(page);
				for (int i = 0; i < BoundCount; i++)
				{
					count = FPDFText_GetBoundedText(text_page,
						CaptureBounds[i].Left, CaptureBounds[i].Top, CaptureBounds[i].Right, CaptureBounds[i].Bottom,
						shortPointer + countSum,
						processInfo->bufLength);
					countSum += count;
					processInfo->textlengths[i] = count;
				}
				FPDFText_ClosePage(text_page);
			}
			FPDF_ClosePage(page);
			FPDF_CloseDocument(doc);
			free(file_contents);
		}

		//ThreadPool pool(8);
		//std::vector<std::future<int>> void_result;
		//for (int i = 0; i < length; ++i)
		//{
		//	//auto processInfo = &(*(processInfos + i));
		//	auto processInfo = &(processInfos[i]);
		//	void_result.emplace_back(
		//		pool.enqueue([processInfo]
		//	{
		//		size_t file_length = 0;
		//		char* file_contents = GetFileContents(processInfo->filename, &file_length);
		//		if (!file_contents)
		//			return 0;
		//		FPDF_DOCUMENT doc = FPDF_LoadMemDocument(file_contents, static_cast<int>(file_length), NULL);
		//		FPDF_PAGE page = FPDF_LoadPage(doc, 0);
		//		int countSum = 0;
		//		int count = 0;
		//		if (page)
		//		{
		//			auto shortPointer = static_cast<unsigned short*>(static_cast<void*>(processInfo->textBuf));
		//			
		//			FPDF_TEXTPAGE text_page = FPDFText_LoadPage(page);
		//			for (int i = 0; i < BoundCount; i++)
		//			{
		//				count = FPDFText_GetBoundedText(text_page,
		//					CaptureBounds[i].Left, CaptureBounds[i].Top, CaptureBounds[i].Right, CaptureBounds[i].Bottom,
		//					shortPointer + countSum,
		//					processInfo->bufLength);
		//				countSum += count;
		//				processInfo->textlengths[i] = count;
		//			}
		//			FPDFText_ClosePage(text_page);
		//		}
		//		FPDF_ClosePage(page);
		//		FPDF_CloseDocument(doc);
		//		free(file_contents);
		//		return countSum;
		//	}));
		//}
		//for (auto && result : void_result)
		//	result.wait();
	}

	DLLEXPORT void ModifyStructValue(struct_Bound* arr, int length)
	{
		for (int i = 0; i < length; i++)
		{
			arr[i].Top = 3;
			printf_s("%f\n", arr[i].Left);
		}
	}

	DLLEXPORT void InitLibrary()
	{
		FPDF_InitLibrary();
	}

	DLLEXPORT void DestroyLibrary()
	{
		FPDF_DestroyLibrary();
	}

	DLLEXPORT bool TestDllWork()
	{
		return true;
	}

	DLLEXPORT int GetBoundedText(const char * filename, byte* textBuf, int bufLength)
	{
		//std::string filename = files.front();
		//files.pop_front();
		size_t file_length = 0;
		char* file_contents = GetFileContents(filename, &file_length);
		if (!file_contents)
			return 0;

		//printf_s("Rendering PDF file %s.\n", name.c_str());
		FPDF_DOCUMENT doc = FPDF_LoadMemDocument(file_contents, static_cast<int>(file_length), NULL);

		FPDF_PAGE page = FPDF_LoadPage(doc, 0);
		int count = 0;


		//unsigned short textTempBuf[65536];
		if (page)
		{

			FPDF_TEXTPAGE text_page = FPDFText_LoadPage(page);
			//count = FPDFText_GetBoundedText(text_page, 5, 5, 600, 600, textTempBuf, bufLength);
			count = FPDFText_GetBoundedText(text_page, 5, 5, 600, 600,
				static_cast<unsigned short*>(static_cast<void*>(textBuf)),
				bufLength);

			//memcpy_s(textBuf, 65536 * 2, textTempBuf, 65536);

			FPDFText_ClosePage(text_page);
		}
		FPDF_ClosePage(page);
		FPDF_CloseDocument(doc);
		free(file_contents);
		return count;
	}

	DLLEXPORT int GetMultiBoundedText(const char * filename, byte* textBuf, int bufLength,
		struct_Bound* bounds, int boundsCount, int* textlengths)
	{
		//std::string filename = files.front();
		//files.pop_front();
		size_t file_length = 0;
		char* file_contents = GetFileContents(filename, &file_length);
		if (!file_contents)
			return 0;

		//printf_s("Rendering PDF file %s.\n", name.c_str());
		FPDF_DOCUMENT doc = FPDF_LoadMemDocument(file_contents, static_cast<int>(file_length), NULL);

		FPDF_PAGE page = FPDF_LoadPage(doc, 0);
		int countSum = 0;
		int count = 0;
		if (page)
		{
			auto shortPointer = static_cast<unsigned short*>(static_cast<void*>(textBuf));
			FPDF_TEXTPAGE text_page = FPDFText_LoadPage(page);
			for (int i = 0; i < boundsCount; i++)
			{
				count = FPDFText_GetBoundedText(text_page,
					bounds[i].Left, bounds[i].Top, bounds[i].Right, bounds[i].Bottom,
					shortPointer + countSum,
					bufLength);
				countSum += count;
				textlengths[i] = count;
			}
			FPDFText_ClosePage(text_page);
		}
		FPDF_ClosePage(page);
		FPDF_CloseDocument(doc);
		free(file_contents);
		return countSum;
	}

	DLLEXPORT int GetMultiBoundedTextFromBinary(unsigned char * filedata, int filelength, unsigned short* textBuf, int bufLength,
		struct_Bound* bounds, int boundsCount, int* textlengths)
	{
		FPDF_DOCUMENT doc = FPDF_LoadMemDocument(filedata, filelength, NULL);

		FPDF_PAGE page = FPDF_LoadPage(doc, 0);
		int countSum = 0;
		int count = 0;
		if (page)
		{
			FPDF_TEXTPAGE text_page = FPDFText_LoadPage(page);
			for (int i = 0; i < boundsCount; i++)
			{
				count = FPDFText_GetBoundedText(text_page,
					bounds[i].Left, bounds[i].Top, bounds[i].Right, bounds[i].Bottom,
					textBuf + countSum, bufLength);
				countSum += count;
				textlengths[i] = count;
			}
			FPDFText_ClosePage(text_page);
		}
		FPDF_ClosePage(page);
		FPDF_CloseDocument(doc);
		return countSum;
	}

	//DLLEXPORT int GetMultiBoundedTextFromBinary(unsigned char * filedata, int filelength, unsigned short* textBuf, int bufLength,
	//	struct_Bound* bounds, int boundsCount, int* textlengths)
	//{
	//	FPDF_DOCUMENT doc = FPDF_LoadMemDocument(filedata, filelength, NULL);
	//	FPDF_PAGE page = FPDF_LoadPage(doc, 0);
	//	int countSum = 0;
	//	int count = 0;
	//	if (page)
	//	{
	//		FPDF_TEXTPAGE text_page = FPDFText_LoadPage(page);
	//		for (int i = 0; i < boundsCount; i++)
	//		{
	//			count = FPDFText_GetBoundedText(text_page,
	//				bounds[i].Left, bounds[i].Top, bounds[i].Right, bounds[i].Bottom,
	//				textBuf + countSum, bufLength);
	//			countSum += count;
	//			textlengths[i] = count;
	//		}
	//		FPDFText_ClosePage(text_page);
	//	}
	//	FPDF_ClosePage(page);
	//	FPDF_CloseDocument(doc);
	//	return countSum;
	//}
}
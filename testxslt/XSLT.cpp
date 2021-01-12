// XSLT.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <tchar.h>
#include <msxml6.h>
#include <iostream>
#include <fstream>

// Macro that calls a COM method returning HRESULT value.
#define CHK_HR(stmt)        do { hr=(stmt); if (FAILED(hr)) goto CleanUp; } while(0)

// Macro to verify memory allcation.
#define CHK_ALLOC(p)        do { if (!(p)) { hr = E_OUTOFMEMORY; goto CleanUp; } } while(0)

// Macro that releases a COM object if not NULL.
#define SAFE_RELEASE(p)     do { if ((p)) { (p)->Release(); (p) = NULL; } } while(0)

// Helper function to create a VT_BSTR variant from a null terminated string. 
HRESULT VariantFromString(PCWSTR wszValue, VARIANT& Variant)
{
    HRESULT hr = S_OK;
    BSTR bstrString = SysAllocString(wszValue);
    CHK_ALLOC(bstrString);

    V_VT(&Variant) = VT_BSTR;
    V_BSTR(&Variant) = bstrString;

CleanUp:
    return hr;
}

// Helper function to create a DOM instance. 
HRESULT CreateAndInitDOM(IXMLDOMDocument** ppDoc)
{
    HRESULT hr = CoCreateInstance(__uuidof(DOMDocument60), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(ppDoc));
    if (SUCCEEDED(hr))
    {
        // these methods should not fail so don't inspect result
        (*ppDoc)->put_async(VARIANT_FALSE);
        (*ppDoc)->put_validateOnParse(VARIANT_FALSE);
        (*ppDoc)->put_resolveExternals(VARIANT_FALSE);
    }
    return hr;
}

// Helper function to load xml from file. 
HRESULT LoadXMLFile(IXMLDOMDocument* pXMLDom, LPCWSTR lpszXMLFile)
{
    HRESULT hr = S_OK;
    VARIANT_BOOL varStatus;
    VARIANT varFileName;
    IXMLDOMParseError* pXMLErr = NULL;
    BSTR bstrErr = NULL;

    VariantInit(&varFileName);
    CHK_HR(VariantFromString(lpszXMLFile, varFileName));
    CHK_HR(pXMLDom->load(varFileName, &varStatus));

    //load xml failed
    if (varStatus != VARIANT_TRUE)
    {
        hr = E_FAIL;
        CHK_HR(pXMLDom->get_parseError(&pXMLErr));
        CHK_HR(pXMLErr->get_reason(&bstrErr));
        printf("Failed to load %S:\n%S\n", lpszXMLFile, bstrErr);
    }

CleanUp:
    SAFE_RELEASE(pXMLErr);
    SysFreeString(bstrErr);
    VariantClear(&varFileName);
    return hr;
}

// Helper function to transform DOM to a string. 
HRESULT TransformDOM2Str(IXMLDOMDocument* pXMLDom, IXMLDOMDocument* pXSLDoc)
{
    HRESULT hr = S_OK;
    BSTR bstrResult = NULL;
    std::wstring ws;
    std::string s;
    std::ofstream mfout("outfile.txt", std::ios::out);

    //
    BSTR text = NULL;
    pXMLDom->get_xml(&text);
    printf("\n\nInput to transformNode:\n%S\n", text);
    //

    CHK_HR(pXMLDom->transformNode(pXSLDoc, &bstrResult));
    printf("\n\nOutput from transformNode:\n%S\n", bstrResult);

    ws = std::wstring(bstrResult, SysStringLen(bstrResult));
    std::wcout << "\n\n\n\nresW:\n" << ws << std::endl;

    s = std::string(ws.begin(), ws.end());
    std::cout << "\n\n\n\nresS:\n" << s << std::endl;

    mfout << s;
    mfout.close();


CleanUp:
    SysFreeString(bstrResult);
    return hr;
}

// Helper function to transform DOM to an object. 
HRESULT TransformDOM2Obj(IXMLDOMDocument* pXMLDom, IXMLDOMDocument* pXSLDoc)
{
    HRESULT hr = S_OK;
    BSTR bstrXML = NULL;
    IXMLDOMDocument* pXMLOut = NULL;
    IDispatch* pDisp = NULL;
    VARIANT varDisp;
    VARIANT varFileName;
    BSTR text = NULL;//

    VariantInit(&varDisp);
    VariantInit(&varFileName);

    CHK_HR(CreateAndInitDOM(&pXMLOut));
    CHK_HR(pXMLOut->QueryInterface(IID_IDispatch, (void**)&pDisp));

    V_VT(&varDisp) = VT_DISPATCH;
    V_DISPATCH(&varDisp) = pDisp;
    pDisp = NULL;

    CHK_HR(pXMLDom->transformNodeToObject(pXSLDoc, varDisp));
    CHK_HR(pXMLOut->get_xml(&bstrXML));
    printf("\n\n\n\nOutput from transformNodeToObject:\n%S\n", bstrXML);

    //save to stocks
    CHK_HR(VariantFromString(L"stocks.txt", varFileName));
    CHK_HR(pXMLOut->save(varFileName));
    printf("\n\n\n\nThe above output is also saved in stocks.txt.\n");

CleanUp:
    VariantClear(&varDisp);
    VariantClear(&varFileName);
    SysFreeString(bstrXML);
    SAFE_RELEASE(pXMLOut);
    return hr;
}

void XSLT()
{
    HRESULT hr = S_OK;
    IXMLDOMDocument* pXMLDom = NULL;
    IXMLDOMDocument* pXSLDoc = NULL;

    CHK_HR(CreateAndInitDOM(&pXMLDom));
    CHK_HR(LoadXMLFile(pXMLDom, L"stocks.xml"));
    CHK_HR(CreateAndInitDOM(&pXSLDoc));
    CHK_HR(LoadXMLFile(pXSLDoc, L"stocks.xslt"));

    // Transform dom to a string:
    CHK_HR(TransformDOM2Str(pXMLDom, pXSLDoc));
    // Transform dom to another dom object:
    //CHK_HR(TransformDOM2Obj(pXMLDom, pXSLDoc));

CleanUp:
    SAFE_RELEASE(pXSLDoc);
    SAFE_RELEASE(pXMLDom);
}

int _tmain(int argc, _TCHAR* argv[])
{
    setlocale(LC_ALL, "Russian");
    HRESULT hr = CoInitialize(NULL);
    if (SUCCEEDED(hr))
    {
        XSLT();
        CoUninitialize();
    }

    return 0;

}
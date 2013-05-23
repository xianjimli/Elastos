
#include "ext/frameworkext.h"
#include "webkit/CMimeTypeMap.h"

//CMimeTypeMap* CMimeTypeMap::sMimeTypeMap;

ECode CMimeTypeMap::GetFileExtensionFromUrl(
    /* [in] */ CString inUrl,
    /* [out] */ String* outUrl)
{
    VALIDATE_NOT_NULL(outUrl);

    String url((const char*)inUrl);

    if (url.GetLength() > 0)
    {
        Int32 query = url.LastIndexOf('?');
        if (query > 0) {
            url = url.Substring(0, query);
        }
        Int32 filenamePos = url.LastIndexOf('/');
        String filename =
            0 <= filenamePos ? url.Substring(filenamePos + 1) : url;

        // if the filename contains special characters, we don't
        // consider it valid for our matching purposes:
        if (filename.GetLength() > 0 /*&&
            Pattern.matches("[a-zA-Z_0-9\\.\\-\\(\\)\\%]+", filename)*/) {
            Int32 dotPos = filename.LastIndexOf('.');
            if (0 <= dotPos) {
                *outUrl = filename.Substring(dotPos + 1);
                return E_NOT_IMPLEMENTED;
            }
        }
    }

    *outUrl = "";

    return NOERROR;
}

ECode CMimeTypeMap::HasMimeType(
    /* [in] */ CString mimeType,
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);

    if (!mimeType.IsNullOrEmpty()) {
        HashMap<String, String>::Iterator it =
            mMimeTypeToExtensionMap.Find(String(mimeType));
        if (it == mMimeTypeToExtensionMap.End()) {
            *flag = FALSE;
            return NOERROR;
        }
        else {
            *flag = TRUE;
            return NOERROR;
        }
    }

    *flag = FALSE;
 
    return NOERROR;
}

ECode CMimeTypeMap::GetMimeTypeFromExtension(
    /* [in] */ CString extension,
    /* [out] */ String* mimeType)
{
    VALIDATE_NOT_NULL(mimeType);

    if (!extension.IsNullOrEmpty()) {
        HashMap<String, String>::Iterator it =
            mExtensionToMimeTypeMap.Find(String(extension));
        if (it == mExtensionToMimeTypeMap.End()) {
            mimeType->SetTo(NULL);
            return NOERROR;
        }
        else {
            *mimeType = it->mSecond;
            return NOERROR;
        }
    }

    mimeType->SetTo(NULL);

    return NOERROR;
}

ECode CMimeTypeMap::HasExtension(
    /* [in] */ CString extension,
    /* [out] */ Boolean* flag)
{
    VALIDATE_NOT_NULL(flag);

    if (!extension.IsNullOrEmpty()) {
        HashMap<String, String>::Iterator it =
            mMimeTypeToExtensionMap.Find(String(extension));
        if (it == mMimeTypeToExtensionMap.End()) {
            *flag = FALSE;
            return NOERROR;
        }
        else {
            *flag = TRUE;
            return NOERROR;
        }
    }

    *flag = FALSE;

    return NOERROR;
}

ECode CMimeTypeMap::GetExtensionFromMimeType(
    /* [in] */ CString mimeType,
    /* [out] */ String* extensionFrom)
{
    VALIDATE_NOT_NULL(extensionFrom);

    if (!mimeType.IsNullOrEmpty()) {
        HashMap<String, String>::Iterator it =
            mMimeTypeToExtensionMap.Find(String(mimeType));
        if (it == mMimeTypeToExtensionMap.End()) {
            extensionFrom->SetTo(NULL);
            return NOERROR;
        }
        else {
            *extensionFrom = it->mSecond;
            return NOERROR;
        }
    }

    extensionFrom->SetTo(NULL);
 
    return NOERROR;
}

ECode CMimeTypeMap::GetSingleton(
    /* [out] */ IMimeTypeMap** instance)
{
#if 0
    VALIDATE_NOT_NULL(instance);

    *instance = this;
    AddRef();
#endif

    return NOERROR;
}

ECode CMimeTypeMap::constructor()
{
    // The following table is based on /etc/mime.types data minus
    // chemical/* MIME types and MIME types that don't map to any
    // file extensions. We also exclude top-level domain names to
    // deal with cases like:
    //
    // mail.google.com/a/google.com
    //
    // and "active" MIME types (due to potential security issues).

    LoadEntry("application/andrew-inset", "ez");
    LoadEntry("application/dsptype", "tsp");
    LoadEntry("application/futuresplash", "spl");
    LoadEntry("application/hta", "hta");
    LoadEntry("application/mac-binhex40", "hqx");
    LoadEntry("application/mac-compactpro", "cpt");
    LoadEntry("application/mathematica", "nb");
    LoadEntry("application/msaccess", "mdb");
    LoadEntry("application/oda", "oda");
    LoadEntry("application/ogg", "ogg");
    LoadEntry("application/pdf", "pdf");
    LoadEntry("application/pgp-keys", "key");
    LoadEntry("application/pgp-signature", "pgp");
    LoadEntry("application/pics-rules", "prf");
    LoadEntry("application/rar", "rar");
    LoadEntry("application/rdf+xml", "rdf");
    LoadEntry("application/rss+xml", "rss");
    LoadEntry("application/zip", "zip");
    LoadEntry("application/vnd.android.package-archive",
            "apk");
    LoadEntry("application/vnd.cinderella", "cdy");
    LoadEntry("application/vnd.ms-pki.stl", "stl");
    LoadEntry(
            "application/vnd.oasis.opendocument.database", "odb");
    LoadEntry(
            "application/vnd.oasis.opendocument.formula", "odf");
    LoadEntry(
            "application/vnd.oasis.opendocument.graphics", "odg");
    LoadEntry(
            "application/vnd.oasis.opendocument.graphics-template",
            "otg");
    LoadEntry(
            "application/vnd.oasis.opendocument.image", "odi");
    LoadEntry(
            "application/vnd.oasis.opendocument.spreadsheet", "ods");
    LoadEntry(
            "application/vnd.oasis.opendocument.spreadsheet-template",
            "ots");
    LoadEntry(
            "application/vnd.oasis.opendocument.text", "odt");
    LoadEntry(
            "application/vnd.oasis.opendocument.text-master", "odm");
    LoadEntry(
            "application/vnd.oasis.opendocument.text-template", "ott");
    LoadEntry(
            "application/vnd.oasis.opendocument.text-web", "oth");
    LoadEntry("application/msword", "doc");
    LoadEntry("application/msword", "dot");
    LoadEntry(
            "application/vnd.openxmlformats-officedocument.wordprocessingml.document",
            "docx");
    LoadEntry(
            "application/vnd.openxmlformats-officedocument.wordprocessingml.template",
            "dotx");
    LoadEntry("application/vnd.ms-excel", "xls");
    LoadEntry("application/vnd.ms-excel", "xlt");
    LoadEntry(
            "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet",
            "xlsx");
    LoadEntry(
            "application/vnd.openxmlformats-officedocument.spreadsheetml.template",
            "xltx");
    LoadEntry("application/vnd.ms-powerpoint", "ppt");
    LoadEntry("application/vnd.ms-powerpoint", "pot");
    LoadEntry("application/vnd.ms-powerpoint", "pps");
    LoadEntry(
            "application/vnd.openxmlformats-officedocument.presentationml.presentation",
            "pptx");
    LoadEntry(
            "application/vnd.openxmlformats-officedocument.presentationml.template",
            "potx");
    LoadEntry(
            "application/vnd.openxmlformats-officedocument.presentationml.slideshow",
            "ppsx");
    LoadEntry("application/vnd.rim.cod", "cod");
    LoadEntry("application/vnd.smaf", "mmf");
    LoadEntry("application/vnd.stardivision.calc", "sdc");
    LoadEntry("application/vnd.stardivision.draw", "sda");
    LoadEntry(
            "application/vnd.stardivision.impress", "sdd");
    LoadEntry(
            "application/vnd.stardivision.impress", "sdp");
    LoadEntry("application/vnd.stardivision.math", "smf");
    LoadEntry("application/vnd.stardivision.writer",
            "sdw");
    LoadEntry("application/vnd.stardivision.writer",
            "vor");
    LoadEntry(
            "application/vnd.stardivision.writer-global", "sgl");
    LoadEntry("application/vnd.sun.xml.calc", "sxc");
    LoadEntry(
            "application/vnd.sun.xml.calc.template", "stc");
    LoadEntry("application/vnd.sun.xml.draw", "sxd");
    LoadEntry(
            "application/vnd.sun.xml.draw.template", "std");
    LoadEntry("application/vnd.sun.xml.impress", "sxi");
    LoadEntry(
            "application/vnd.sun.xml.impress.template", "sti");
    LoadEntry("application/vnd.sun.xml.math", "sxm");
    LoadEntry("application/vnd.sun.xml.writer", "sxw");
    LoadEntry(
            "application/vnd.sun.xml.writer.global", "sxg");
    LoadEntry(
            "application/vnd.sun.xml.writer.template", "stw");
    LoadEntry("application/vnd.visio", "vsd");
    LoadEntry("application/x-abiword", "abw");
    LoadEntry("application/x-apple-diskimage", "dmg");
    LoadEntry("application/x-bcpio", "bcpio");
    LoadEntry("application/x-bittorrent", "torrent");
    LoadEntry("application/x-cdf", "cdf");
    LoadEntry("application/x-cdlink", "vcd");
    LoadEntry("application/x-chess-pgn", "pgn");
    LoadEntry("application/x-cpio", "cpio");
    LoadEntry("application/x-debian-package", "deb");
    LoadEntry("application/x-debian-package", "udeb");
    LoadEntry("application/x-director", "dcr");
    LoadEntry("application/x-director", "dir");
    LoadEntry("application/x-director", "dxr");
    LoadEntry("application/x-dms", "dms");
    LoadEntry("application/x-doom", "wad");
    LoadEntry("application/x-dvi", "dvi");
    LoadEntry("application/x-flac", "flac");
    LoadEntry("application/x-font", "pfa");
    LoadEntry("application/x-font", "pfb");
    LoadEntry("application/x-font", "gsf");
    LoadEntry("application/x-font", "pcf");
    LoadEntry("application/x-font", "pcf.Z");
    LoadEntry("application/x-freemind", "mm");
    LoadEntry("application/x-futuresplash", "spl");
    LoadEntry("application/x-gnumeric", "gnumeric");
    LoadEntry("application/x-go-sgf", "sgf");
    LoadEntry("application/x-graphing-calculator", "gcf");
    LoadEntry("application/x-gtar", "gtar");
    LoadEntry("application/x-gtar", "tgz");
    LoadEntry("application/x-gtar", "taz");
    LoadEntry("application/x-hdf", "hdf");
    LoadEntry("application/x-ica", "ica");
    LoadEntry("application/x-internet-signup", "ins");
    LoadEntry("application/x-internet-signup", "isp");
    LoadEntry("application/x-iphone", "iii");
    LoadEntry("application/x-iso9660-image", "iso");
    LoadEntry("application/x-jmol", "jmz");
    LoadEntry("application/x-kchart", "chrt");
    LoadEntry("application/x-killustrator", "kil");
    LoadEntry("application/x-koan", "skp");
    LoadEntry("application/x-koan", "skd");
    LoadEntry("application/x-koan", "skt");
    LoadEntry("application/x-koan", "skm");
    LoadEntry("application/x-kpresenter", "kpr");
    LoadEntry("application/x-kpresenter", "kpt");
    LoadEntry("application/x-kspread", "ksp");
    LoadEntry("application/x-kword", "kwd");
    LoadEntry("application/x-kword", "kwt");
    LoadEntry("application/x-latex", "latex");
    LoadEntry("application/x-lha", "lha");
    LoadEntry("application/x-lzh", "lzh");
    LoadEntry("application/x-lzx", "lzx");
    LoadEntry("application/x-maker", "frm");
    LoadEntry("application/x-maker", "maker");
    LoadEntry("application/x-maker", "frame");
    LoadEntry("application/x-maker", "fb");
    LoadEntry("application/x-maker", "book");
    LoadEntry("application/x-maker", "fbdoc");
    LoadEntry("application/x-mif", "mif");
    LoadEntry("application/x-ms-wmd", "wmd");
    LoadEntry("application/x-ms-wmz", "wmz");
    LoadEntry("application/x-msi", "msi");
    LoadEntry("application/x-ns-proxy-autoconfig", "pac");
    LoadEntry("application/x-nwc", "nwc");
    LoadEntry("application/x-object", "o");
    LoadEntry("application/x-oz-application", "oza");
    LoadEntry("application/x-pkcs12", "p12");
    LoadEntry("application/x-pkcs7-certreqresp", "p7r");
    LoadEntry("application/x-pkcs7-crl", "crl");
    LoadEntry("application/x-quicktimeplayer", "qtl");
    LoadEntry("application/x-shar", "shar");
    LoadEntry("application/x-shockwave-flash", "swf");
    LoadEntry("application/x-stuffit", "sit");
    LoadEntry("application/x-sv4cpio", "sv4cpio");
    LoadEntry("application/x-sv4crc", "sv4crc");
    LoadEntry("application/x-tar", "tar");
    LoadEntry("application/x-texinfo", "texinfo");
    LoadEntry("application/x-texinfo", "texi");
    LoadEntry("application/x-troff", "t");
    LoadEntry("application/x-troff", "roff");
    LoadEntry("application/x-troff-man", "man");
    LoadEntry("application/x-ustar", "ustar");
    LoadEntry("application/x-wais-source", "src");
    LoadEntry("application/x-wingz", "wz");
    LoadEntry("application/x-webarchive", "webarchive");
    LoadEntry("application/x-x509-ca-cert", "crt");
    LoadEntry("application/x-x509-user-cert", "crt");
    LoadEntry("application/x-xcf", "xcf");
    LoadEntry("application/x-xfig", "fig");
    LoadEntry("application/xhtml+xml", "xhtml");
    LoadEntry("audio/3gpp", "3gpp");
    LoadEntry("audio/basic", "snd");
    LoadEntry("audio/midi", "mid");
    LoadEntry("audio/midi", "midi");
    LoadEntry("audio/midi", "kar");
    LoadEntry("audio/mpeg", "mpga");
    LoadEntry("audio/mpeg", "mpega");
    LoadEntry("audio/mpeg", "mp2");
    LoadEntry("audio/mpeg", "mp3");
    LoadEntry("audio/mpeg", "m4a");
    LoadEntry("audio/mpegurl", "m3u");
    LoadEntry("audio/prs.sid", "sid");
    LoadEntry("audio/x-aiff", "aif");
    LoadEntry("audio/x-aiff", "aiff");
    LoadEntry("audio/x-aiff", "aifc");
    LoadEntry("audio/x-gsm", "gsm");
    LoadEntry("audio/x-mpegurl", "m3u");
    LoadEntry("audio/x-ms-wma", "wma");
    LoadEntry("audio/x-ms-wax", "wax");
    LoadEntry("audio/x-pn-realaudio", "ra");
    LoadEntry("audio/x-pn-realaudio", "rm");
    LoadEntry("audio/x-pn-realaudio", "ram");
    LoadEntry("audio/x-realaudio", "ra");
    LoadEntry("audio/x-scpls", "pls");
    LoadEntry("audio/x-sd2", "sd2");
    LoadEntry("audio/x-wav", "wav");
    LoadEntry("image/bmp", "bmp");
    LoadEntry("image/gif", "gif");
    LoadEntry("image/ico", "cur");
    LoadEntry("image/ico", "ico");
    LoadEntry("image/ief", "ief");
    LoadEntry("image/jpeg", "jpeg");
    LoadEntry("image/jpeg", "jpg");
    LoadEntry("image/jpeg", "jpe");
    LoadEntry("image/pcx", "pcx");
    LoadEntry("image/png", "png");
    LoadEntry("image/svg+xml", "svg");
    LoadEntry("image/svg+xml", "svgz");
    LoadEntry("image/tiff", "tiff");
    LoadEntry("image/tiff", "tif");
    LoadEntry("image/vnd.djvu", "djvu");
    LoadEntry("image/vnd.djvu", "djv");
    LoadEntry("image/vnd.wap.wbmp", "wbmp");
    LoadEntry("image/x-cmu-raster", "ras");
    LoadEntry("image/x-coreldraw", "cdr");
    LoadEntry("image/x-coreldrawpattern", "pat");
    LoadEntry("image/x-coreldrawtemplate", "cdt");
    LoadEntry("image/x-corelphotopaint", "cpt");
    LoadEntry("image/x-icon", "ico");
    LoadEntry("image/x-jg", "art");
    LoadEntry("image/x-jng", "jng");
    LoadEntry("image/x-ms-bmp", "bmp");
    LoadEntry("image/x-photoshop", "psd");
    LoadEntry("image/x-portable-anymap", "pnm");
    LoadEntry("image/x-portable-bitmap", "pbm");
    LoadEntry("image/x-portable-graymap", "pgm");
    LoadEntry("image/x-portable-pixmap", "ppm");
    LoadEntry("image/x-rgb", "rgb");
    LoadEntry("image/x-xbitmap", "xbm");
    LoadEntry("image/x-xpixmap", "xpm");
    LoadEntry("image/x-xwindowdump", "xwd");
    LoadEntry("model/iges", "igs");
    LoadEntry("model/iges", "iges");
    LoadEntry("model/mesh", "msh");
    LoadEntry("model/mesh", "mesh");
    LoadEntry("model/mesh", "silo");
    LoadEntry("text/calendar", "ics");
    LoadEntry("text/calendar", "icz");
    LoadEntry("text/comma-separated-values", "csv");
    LoadEntry("text/css", "css");
    LoadEntry("text/html", "htm");
    LoadEntry("text/html", "html");
    LoadEntry("text/h323", "323");
    LoadEntry("text/iuls", "uls");
    LoadEntry("text/mathml", "mml");
    // add it first so it will be the default for ExtensionFromMimeType
    LoadEntry("text/plain", "txt");
    LoadEntry("text/plain", "asc");
    LoadEntry("text/plain", "text");
    LoadEntry("text/plain", "diff");
    LoadEntry("text/plain", "po");     // reserve "pot" for vnd.ms-powerpoint
    LoadEntry("text/richtext", "rtx");
    LoadEntry("text/rtf", "rtf");
    LoadEntry("text/texmacs", "ts");
    LoadEntry("text/text", "phps");
    LoadEntry("text/tab-separated-values", "tsv");
    LoadEntry("text/xml", "xml");
    LoadEntry("text/x-bibtex", "bib");
    LoadEntry("text/x-boo", "boo");
    LoadEntry("text/x-c++hdr", "h++");
    LoadEntry("text/x-c++hdr", "hpp");
    LoadEntry("text/x-c++hdr", "hxx");
    LoadEntry("text/x-c++hdr", "hh");
    LoadEntry("text/x-c++src", "c++");
    LoadEntry("text/x-c++src", "cpp");
    LoadEntry("text/x-c++src", "cxx");
    LoadEntry("text/x-chdr", "h");
    LoadEntry("text/x-component", "htc");
    LoadEntry("text/x-csh", "csh");
    LoadEntry("text/x-csrc", "c");
    LoadEntry("text/x-dsrc", "d");
    LoadEntry("text/x-haskell", "hs");
    LoadEntry("text/x-java", "java");
    LoadEntry("text/x-literate-haskell", "lhs");
    LoadEntry("text/x-moc", "moc");
    LoadEntry("text/x-pascal", "p");
    LoadEntry("text/x-pascal", "pas");
    LoadEntry("text/x-pcs-gcd", "gcd");
    LoadEntry("text/x-setext", "etx");
    LoadEntry("text/x-tcl", "tcl");
    LoadEntry("text/x-tex", "tex");
    LoadEntry("text/x-tex", "ltx");
    LoadEntry("text/x-tex", "sty");
    LoadEntry("text/x-tex", "cls");
    LoadEntry("text/x-vcalendar", "vcs");
    LoadEntry("text/x-vcard", "vcf");
    LoadEntry("video/3gpp", "3gpp");
    LoadEntry("video/3gpp", "3gp");
    LoadEntry("video/3gpp", "3g2");
    LoadEntry("video/dl", "dl");
    LoadEntry("video/dv", "dif");
    LoadEntry("video/dv", "dv");
    LoadEntry("video/fli", "fli");
    LoadEntry("video/m4v", "m4v");
    LoadEntry("video/mpeg", "mpeg");
    LoadEntry("video/mpeg", "mpg");
    LoadEntry("video/mpeg", "mpe");
    LoadEntry("video/mp4", "mp4");
    LoadEntry("video/mpeg", "VOB");
    LoadEntry("video/quicktime", "qt");
    LoadEntry("video/quicktime", "mov");
    LoadEntry("video/vnd.mpegurl", "mxu");
    LoadEntry("video/x-la-asf", "lsf");
    LoadEntry("video/x-la-asf", "lsx");
    LoadEntry("video/x-mng", "mng");
    LoadEntry("video/x-ms-asf", "asf");
    LoadEntry("video/x-ms-asf", "asx");
    LoadEntry("video/x-ms-wm", "wm");
    LoadEntry("video/x-ms-wmv", "wmv");
    LoadEntry("video/x-ms-wmx", "wmx");
    LoadEntry("video/x-ms-wvx", "wvx");
    LoadEntry("video/x-msvideo", "avi");
    LoadEntry("video/x-sgi-movie", "movie");
    LoadEntry("x-conference/x-cooltalk", "ice");
    LoadEntry("x-epoc/x-sisx-app", "sisx");

    return NOERROR;
}

void CMimeTypeMap::LoadEntry(
    /* [in] */ const char* mimeType,
    /* [in] */ const char* extension)
{
    //
    // if we have an existing x --> y mapping, we do not want to
    // override it with another mapping x --> ?
    // this is mostly because of the way the mime-type map below
    // is constructed (if a mime type maps to several extensions
    // the first extension is considered the most popular and is
    // added first; we do not want to overwrite it later).
    //

    if (mimeType == NULL || extension == NULL) {
        return;
    }

    if (mMimeTypeToExtensionMap.Find(String(mimeType))
        != mMimeTypeToExtensionMap.End()) {
        mMimeTypeToExtensionMap[String(mimeType)] = String(extension);
    }

    mExtensionToMimeTypeMap[String(extension)] = String(mimeType);
}
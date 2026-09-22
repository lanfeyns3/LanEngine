#include "File.h"

namespace LANE
{
    std::expected<ReadResult,FileLoadError> File::Read(FileLoadType type)
    {
        switch (type)
        {
        case FileLoadType::Json:
        {
            std::ifstream f(m_path);
            if (!f.is_open())
                return std::unexpected(FileLoadError::FileCouldNotLoad);
            
            nlohmann::json file = nlohmann::json::parse(f);

            return file;
            break;
        }
        
        default:
            break;
        }
    }

    void File::Write(std::string data)
    {
        std::fstream f(m_path,std::ios::out);

        f << data;
        f.close();
    }

    std::string File::PromptFileDialog(DialogType type,std::vector<nfdu8filteritem_t> filters)
    {
        switch (type)
        {
        case DialogType::Open:
            
            break;
        case DialogType::Save:
        {
            nfdu8char_t *outPath;
            nfdsavedialogu8args_t args{0};
            args.filterList = filters.data();
            args.filterCount = filters.size();
            
            nfdresult_t result = NFD_SaveDialogU8_With(&outPath,&args);
            
            switch (result)
            {
            case NFD_OKAY:
                m_path = outPath;
                NFD_FreePathU8(outPath);
                break;
            
            default:
                break;
            }
        
            break;
        }
        default:
            break;
        }

        return m_path;
    }

} // namespace LANE

namespace Common {

    Item::ID Item::CurrentID = 0;

    Item::Item(const std::string& path) : m_ID(CurrentID++) {

        if(!std::filesystem::exists(path)) Fatal(fmt::format("No such file {}", path));

        YAML::Node map = YAML::LoadFile(path);

        m_Name = map["name"].as<std::string>("Unnamed Item");
        m_Description = map["description"].as<std::string>("There is quite literally nothing here");

        auto usage = map["usage"].as<std::string>("Reusable");
        if(usage == "Consumable") m_Usage = Item::Usage::Consumable;
        else if(usage == "Reusable") m_Usage = Item::Usage::Reusable;
        else if(usage == "Equipable") m_Usage = Item::Usage::Equipable;
        else Fatal(fmt::format("Invalid usage `{}` in item `{}`", usage, path));

        auto slot = map["slot"].as<std::string>("None");
        if(slot == "None") m_Slot = Item::Slot::None;
        else if(slot == "Armor") m_Slot = Item::Slot::Armor;
        else if(slot == "Weapon") m_Slot = Item::Slot::Weapon;
        else Fatal(fmt::format("Invalid slot `{}` in item `{}`", slot, path));

        auto rarity = map["rarity"].as<std::string>("Common");
        if(rarity == "Common") m_Rarity = Item::Rarity::Common;
        else if(rarity == "Uncommon") m_Rarity = Item::Rarity::Uncommon;
        else if(rarity == "Rare") m_Rarity = Item::Rarity::Rare;
        else Fatal(fmt::format("Invalid rarity `{}` in item `{}`", rarity, path));

    }

}

/// Dump vtable in order
std::vector<VirtualInvokeData> vtable;
std::vector<Il2CppRuntimeInterfaceOffsetPair> offsets;
DefaultCSlider::___TypeRegistration::get()->getVtable(vtable, offsets);

for (int i = 0; i < vtable.size(); ++i)
    ChatPlexSDK::Logger()->Error(u"Method " + std::to_string(i) + " is " + vtable[i].method->name);
// STLScanner.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <algorithm>

#include "Vertex.h"
#include "Facet.h"

constexpr uint32_t triangle_size = 50;

constexpr float PI = 3.1415926535897932;

const Vertex* largestNormal = nullptr;
float max_count = 0;

auto updateDefaultAxis(std::unordered_map<Vertex, float, my_string_hash>& counter, const Vertex& axis, float component, Facet& face) -> void
{
    if (axis == face.normal)
    {
        return;
    }
    
    if (component < face.normal.magnitude() * 0.22)
    {
        return;
    }
    
    auto defaultAxis = counter.find(axis);
    (defaultAxis->second) += face.Area() * (component);
    if (defaultAxis->second > max_count)
    {
        largestNormal = (&defaultAxis->first);
        max_count = defaultAxis->second;
    }
    //max_count = defaultAxis->second > max_count ? defaultAxis->second : max_count;
}

auto hexVal(const std::string& hexVal) -> int
{
    int rt = 0;
    int power = 0;
    for (int i = 0; i < hexVal.length(); i++)
    {
        int digit = 0;
        if (hexVal[i] > 96 && hexVal[i] < 123)
        {
            digit = hexVal[i] - 'a' + 10;
        }
        else
        {
            digit = hexVal[i] - '0';
        }
        rt += digit * int(pow(16, power));
        power++;
    }
    return rt;
}

int main()
{   
    std::unordered_map<Vertex,float, my_string_hash> counter;
    std::ifstream ifs("C:/Users/arthu/Desktop/CAD/Trying Times/Break_Carrier_TEBin.stl", std::ios::out | std::ios::binary);
    if (!ifs) {
        std::cout << "Cannot open file!" << std::endl;
        return 1;
    }
    std::cout << "Readinf giel!\n";
    std::filebuf* pbuf = ifs.rdbuf();
    auto size = pbuf->pubseekoff(0, ifs.end);
    // move to beginning of file
    pbuf->pubseekpos(0);
    char* buffer = new char[(size_t)size];
    pbuf->sgetn(buffer, size);
    char* bufptr = buffer;
    bufptr += 80; // skip 80 char header
    size_t triangles = 0;
    int power = 0;
    std::stringstream flipped_nums;
    std::stringstream nums;
    for (int i = 0; i < 4; i++)
    {
        if (*bufptr == '\0')
        {
            bufptr++;
            //power += 2;
            continue;
        }
        //triangles += uint32_t(*bufptr) * uint32_t(pow(16,power));
        nums << std::hex << (static_cast<uint16_t>(*bufptr) & 0xFF);
        std::string temp = nums.str();
        std::reverse(temp.begin(), temp.end());
        flipped_nums << temp;
        nums.str("");
        bufptr++;
        //power += 2;
    }
    std::string hex_string = flipped_nums.str();
    triangles = hexVal(hex_string);
    /*
    std::string convert_to_hex = nums.str();
    std::reverse(convert_to_hex.begin(),convert_to_hex.end());
    std::cout << convert_to_hex << " is convert to hex\n";
    nums.str("");
    for (const auto& item : convert_to_hex)
    {
        uint32_t val = uint32_t(item); // issue with large numbers of polygons having ASCII values out of bounds (i.e. 8C = 140 > 127)
        nums << std::hex << val;
    }
    std::string temp = nums.str();
    triangles = hexVal(temp);
    */
    //bufptr += 4;
    std::cout << triangles << "\n";

    Vertex x_norm(1.0, 0.0, 0.0); // default axes
    Vertex y_norm(0.0, 1.0, 0.0);
    Vertex z_norm(0.0, 0.0, 1.0);
    counter.insert(std::make_pair(x_norm, 0));
    counter.insert(std::make_pair(y_norm, 0));
    counter.insert(std::make_pair(z_norm, 0));
    for (uint32_t i = 0; i < triangles; i++)
    {
        Facet current = reinterpret_cast<Facet&>(*bufptr);
        float Area = current.Area();

        auto itr = counter.find(current.normal);
        if (itr != counter.end())
        {
            (itr->second)+=Area;
            if (itr->second > max_count)
            {
                largestNormal = (&itr->first);
                max_count = itr->second;
            }
            //max_count = itr->second > max_count ? itr->second : max_count;
        }
        else
        {
            
            counter.insert(std::make_pair(current.normal,Area));
            if (Area > max_count)
            {
                largestNormal = (&current.normal);
                max_count = Area;
            }
            //max_count = Area > max_count ? Area : max_count;
        }
        updateDefaultAxis(counter, x_norm, current.normal.x, current);
        updateDefaultAxis(counter, y_norm, current.normal.y, current);
        updateDefaultAxis(counter, z_norm, current.normal.z, current);
        //std::cout << current.a.x << "\n";
        bufptr += triangle_size;
    }
    auto test = counter.find(x_norm);
    std::vector<std::string> largestWeights;
    std::vector<float> sortedWeights;
    float smax = 0;
    Vertex* second_max = nullptr;
    for (auto itr = counter.begin(); itr != counter.end(); itr++)
    {
        //std::string new_str = std::to_string(itr->first.x) + "," + std::to_string(itr->first.y) + "," + std::to_string(itr->first.z) + ": " + std::to_string(itr->second);
        //std::cout << new_str << "\n";
        sortedWeights.push_back(itr->second);
        if (itr->second < max_count && itr->second > smax && !compare_float(itr->second,max_count))
        {
            smax = itr->second;
            second_max = const_cast<Vertex*>(&itr->first);
        }
    }
    std::sort(sortedWeights.begin(), sortedWeights.end());
    /*
    for (size_t i = 0; i < sortedWeights.size(); i++)
    {
        std::cout << sortedWeights[i] << "\n";
    } 
    */
    auto itr = counter.find(x_norm);
    std::cout << itr->second << " is x norm weight\n";
    itr = counter.find(y_norm);
    std::cout << itr->second << " is y norm weight\n";
    itr = counter.find(z_norm);
    std::cout << itr->second << " is z norm weight\n";
    // https://stackoverflow.com/questions/43623549/how-to-rotate-a-vector-around-a-particular-axis // Rodrigues's rotation formula about unit vector w and angle theta
    /*
    Vertex test1(1, 0, 0.5);
    Vertex test2(1, -0.5, -0.5);
    Vertex test3(1, 0.5, -0.5);
    Vertex vect1 = test1 - test2;
    Vertex vect2 = test2 - test3;
    Facet initial(test1, test2, test3);
    initial.normal = cross_Product(vect1, vect2);
    Vertex axis0(1,0,0);
    std::cout << initial.normal.x << "," << initial.normal.y << "," << initial.normal.z << "\n";
    initial.RotateFacet(axis0, PI/3);
    std::cout << initial.normal.x << "," << initial.normal.y << "," << initial.normal.z << "\n";
    */
    std::cout << "Second Largest Normal is:\n";
    std::cout << second_max->x << "," << second_max->y << "," << second_max->z << " with count of " << smax << "\n";

    std::cout << "Largest Normal is:\n";
    std::cout << largestNormal->x << "," << largestNormal->y << "," << largestNormal->z << " with count of " << max_count << "\n";
    // check for ~10% diff, scan triangles for faces whose normals are exactly the same as the two highest and grab the maximum of those two
    bool flip = false;
    double second_sign = 0;
    double max_sign = 0;
    if (max_count * 0.9 < smax)
    {
        max_count = 0;
        smax = 0;
        bufptr = buffer;
        bufptr += 80 + 4;
        for (size_t i = 0; i < triangles; i++)
        {
            Facet current = reinterpret_cast<Facet&>(*bufptr);
            if (current.normal == *second_max)
            {
                float area = current.Area();
                area *= area;
                second_sign += area * current.normal.get_sign(*second_max);
                smax += area;
            }
            else if (current.normal == *largestNormal)
            {
                float area = current.Area();
                area *= area;
                max_sign += area * current.normal.get_sign(*largestNormal);
                max_count += area;
            }
            bufptr += triangle_size;
        }
    }
    
    Vertex* new_norm = nullptr;
    if (smax > max_count)
    {
        new_norm = second_max;
        if (second_sign > 0)
        {
            // we want the new norm to point straight down, so we flip if the majority of faces in
            // this direction point in the current direction of second_max (largest faces go on buildplate)
            *new_norm * -1;
        }
    }
    else
    {
        new_norm = const_cast<Vertex*>(largestNormal);
        if (max_sign > 0)
        {
            *new_norm * -1;
        }
    }
    if (*new_norm == z_norm)
    {
        return 0;
    }
    float left_dot = new_norm->x * z_norm.x + new_norm->y * z_norm.y + new_norm->z * z_norm.z;
    float angle = acos(left_dot / (new_norm->magnitude() * z_norm.magnitude()));
    const Vertex axis_of_rotation = cross_Product(*new_norm, z_norm);
    std::ofstream outfile("C:/Users/arthu/Desktop/CAD/Trying Times/ASCIIRotate.obj");
    //outfile << "solid ASCII\n";
    bufptr = buffer;
    bufptr += 80 + 4;
    std::stringstream faces_;
    //Vertex* vect1 = nullptr;
    //Vertex* vect2 = nullptr;
    uint32_t face_num = 1;
    for (uint32_t i = 0; i < triangles; i++)
    {
        Facet current = reinterpret_cast<Facet&>(*bufptr);
        current.RotateFacet(axis_of_rotation, angle);
        //vect1 = &(current.a - current.b);
        //vect2 = &(current.b - current.c);
        outfile << "v " << current.a.x << ' ' << current.a.y << ' ' << current.a.z << '\n';
        outfile << "v " << current.b.x << ' ' << current.b.y << ' ' << current.b.z << '\n';
        outfile << "v " << current.c.x << ' ' << current.c.y << ' ' << current.c.z << '\n';
        faces_ << "f " << ((int)face_num) << " " << ((int)face_num + 1) << " " << ((int)face_num + 2) << '\n';
        face_num += 3;
        bufptr += triangle_size;
    }
    outfile << faces_.str();
    outfile.close();
    /*
    float test[13];
    memset(test, 0, sizeof(float) * 13);
    //memset(&test[0], 1.0, sizeof(float));
    test[0] = 1;
    test[1] = 0;
    test[2] = 0;
    test[3] = 0;
    test[4] = 0;
    test[5] = 0;
    test[6] = 0;
    test[7] = 1;
    test[8] = 0;
    test[9] = 0;
    test[10] = 0;
    test[11] = 1;
    test[12] = 0;
    //test[13] = 0;
    
    Vertex a(0, 0, 0);
    Vertex b(0, 1, 0);
    Vertex c(0, 0, 1);
    Facet(a, b, c, 1, 0, 0);
    
    Facet a = reinterpret_cast<Facet&>(test);
    std::cout << test[0] << std::endl;
    */
    delete buffer;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

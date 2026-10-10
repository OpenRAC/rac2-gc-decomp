#!/usr/bin/env python3
"""
Import symbol addresses from Going Native (RC2-Going-Decompiled) into symbol_addrs/.

This script fetches symbol addresses from the RC2-Going-Decompiled repository
and imports them into the local symbol_addrs/ directory with proper formatting.
"""

import json
import os
import re


def fetch_ghithub_api(url):
    """Fetch data from GitHub API."""
    import urllib.request
    import urllib.error
    
    try:
        req = urllib.request.Request(url)
        req.add_header('Accept', 'application/vnd.github.v3+json')
        with urllib.request.urlopen(req, timeout=30) as response:
            return json.loads(response.read().decode('utf-8'))
    except Exception as e:
        print(f"Error fetching {url}: {e}")
        return None


def import_symbol_addrs():
    """
    Import symbol addresses from Going Native repository.
    """
    import urllib.request
    
    base_url = "https://api.github.com/repos/Promises/RC2-Going-Decompiled/contents/going-decompiled/symbol_addrs"
    
    # Import USA symbols
    usa_url = f"{base_url}/usa?ref=main"
    print("Fetching USA symbol addresses...")
    usa_data = fetch_ghithub_api(usa_url)
    
    if usa_data is None:
        print("Failed to fetch USA symbols")
        return
    
    usa_symbols = []
    for item in usa_data:
        if item['name'] == 'symbol_addrs.txt':
            # Fetch the actual file content
            content_url = item['download_url']
            try:
                import urllib.request
                with urllib.request.urlopen(content_url, timeout=30) as response:
                    content = response.read().decode('utf-8')
                    
                    # Parse symbol lines
                    for line in content.split('\n'):
                        line = line.strip()
                        if not line or line.startswith('#'):
                            continue
                        
                        # Parse: symbol_name = 0xaddress; // comment
                        match = re.match(r'^(\w+)\s*=\s*(0x[0-9a-fA-F]+)\s*;', line)
                        if match:
                            symbol_name = match.group(1)
                            address = match.group(2).lower()
                            parts = line.split(';')
                            if len(parts) > 1:
                                comment = parts[1].strip()
                                # Remove any leading slashes and space to avoid double slashes
                                if comment.startswith('//'):
                                    comment = comment[2:].strip()
                            else:
                                comment = ''
                            usa_symbols.append({
                                'name': symbol_name,
                                'address': address,
                                'comment': comment
                            })
            
            except Exception as e:
                print(f"Error fetching USA symbol file: {e}")
                continue
    
    print(f"Imported {len(usa_symbols)} USA symbols")
    
    # Write to symbol_addrs/usa/symbol_addrs.txt
    os.makedirs("symbol_addrs/usa", exist_ok=True)
    with open("symbol_addrs/usa/symbol_addrs.txt", 'w') as f:
        f.write("# symbol_addrs.txt - USA region (SCUS_972.68)\n\n")
        f.write("# Symbol addresses imported from Going Native (RC2-Going-Decompiled)\n")
        f.write("# See https://github.com/Promises/RC2-Going-Decompiled\n")
        f.write("# Additional symbols should be added by region and address range.\n\n")
        
        for sym in usa_symbols:
            f.write(f"{sym['name']} = {sym['address']};")
            if sym['comment']:
                f.write(f" // {sym['comment']}")
            f.write("\n")
    
    print(f"Written to symbol_addrs/usa/symbol_addrs.txt")
    
    # Import EU symbols
    eu_url = f"{base_url}/eu?ref=main"
    print("\nFetching EU symbol addresses...")
    eu_data = fetch_ghithub_api(eu_url)
    
    if eu_data is None:
        print("Failed to fetch EU symbols")
        return
    
    eu_symbols = []
    for item in eu_data:
        if item['name'] == 'symbol_addrs.txt':
            content_url = item['download_url']
            try:
                with urllib.request.urlopen(content_url, timeout=30) as response:
                    content = response.read().decode('utf-8')
                    
                    for line in content.split('\n'):
                        line = line.strip()
                        if not line or line.startswith('#'):
                            continue
                        
                        match = re.match(r'^(\w+)\s*=\s*(0x[0-9a-fA-F]+)\s*;', line)
                        if match:
                            symbol_name = match.group(1)
                            address = match.group(2).lower()
                            parts = line.split(';')
                            if len(parts) > 1:
                                comment = parts[1].strip()
                                # Remove any leading slashes and space to avoid double slashes
                                if comment.startswith('//'):
                                    comment = comment[2:].strip()
                            else:
                                comment = ''
                            eu_symbols.append({
                                'name': symbol_name,
                                'address': address,
                                'comment': comment
                            })
            
            except Exception as e:
                print(f"Error fetching EU symbol file: {e}")
                continue
    
    print(f"Imported {len(eu_symbols)} EU symbols")
    
    # Write to symbol_addrs/eu/symbol_addrs.txt
    os.makedirs("symbol_addrs/eu", exist_ok=True)
    with open("symbol_addrs/eu/symbol_addrs.txt", 'w') as f:
        f.write("# symbol_addrs.txt - EU region (SCES-51607)\n\n")
        f.write("# Symbol addresses imported from Going Native (RC2-Going-Decompiled)\n")
        f.write("# See https://github.com/Promises/RC2-Going-Decompiled\n")
        f.write("# Additional symbols should be added by region and address range.\n\n")
        
        for sym in eu_symbols:
            f.write(f"{sym['name']} = {sym['address']};")
            if sym['comment']:
                f.write(f" // {sym['comment']}")
            f.write("\n")
    
    print(f"Written to symbol_addrs/eu/symbol_addrs.txt")
    
    # Summary
    print("\n=== Summary ===")
    print(f"USA symbols imported: {len(usa_symbols)}")
    print(f"EU symbols imported: {len(eu_symbols)}")
    print("\nKey symbols imported:")
    
    # Show some notable symbols
    notable = ['memcpy', 'memset', 'memcmp', 'MapInit', 'GuiManagerCreate', 
               'CollectBolts', 'LoadLevelAndInitHealth', 'SaveLoadStateMachine']
    
    for sym_name in notable:
        usa_sym = next((s for s in usa_symbols if s['name'] == sym_name), None)
        eu_sym = next((s for s in eu_symbols if s['name'] == sym_name), None)
        
        if usa_sym:
            print(f"  USA {sym_name}: {usa_sym['address']}")
        if eu_sym:
            print(f"  EU {sym_name}: {eu_sym['address']}")


if __name__ == '__main__':
    import_symbol_addrs()

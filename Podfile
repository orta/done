target 'Done' do
  pod 'Realm'

  # Has 3 warnings about deprecated -[NSImage setFlipped:] API
  pod 'Fragaria', :podspec => 'Done/Fragaria.podspec.json', :inhibit_warnings => true
  pod 'Sparkle'
  pod 'LetsMove'
end

post_install do |installer|
    # We don't need all of Fragaria's syntax definitions.
    installer.aggregate_targets.each do |aggregate_target|
        script = aggregate_target.copy_resources_script_path
        content = script.read
        content.gsub!('install_resource "Fragaria/Syntax Definitions"', '')
        content.gsub!('install_resource "Fragaria/SyntaxDefinitions.plist"', '')
        script.open('w') { |f| f << content }
    end
end
